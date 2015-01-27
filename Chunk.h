#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED

#include <glm/detail/type_vec4.hpp>

#define BLOCK_X 16
#define BLOCK_Y 16
#define BLOCK_Z 16

extern GLuint attrib_coord;

typedef glm::tvec4<GLbyte> byte4;

static const int transparent[16] = {2, 0, 0, 0, 1, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0};

struct Chunk {
    uint8_t block[BLOCK_X][BLOCK_Y][BLOCK_Z];
    GLuint vbo;
    int elements;
    int changed;

    Chunk() {
        memset(block, 0, sizeof(block));
        elements = 0;
        changed = 1;
        fprintf(stderr, "chunk initiliazed\n");

        exitOnGLError("error before vbo gen");
        glGenBuffers(1, &vbo);
        exitOnGLError("error getting vbo");
    }

    ~Chunk() {
        glDeleteBuffers(1, &vbo);
        fprintf(stderr, "chunk vbo released");
    }

    uint8_t get(int x, int y, int z) {
        return block[x][y][z];
    }

    void set(int x, int y, int z, uint8_t type) {
        block[x][y][z] = type;
        changed = 1;
        fprintf(stderr, "setting data on chunk\n");
    }

    bool isblocked(int x1, int y1, int z1, int x2, int y2, int z2) {
        // Invisible blocks are always "blocked"
        if(!block[x1][y1][z1])
            return true;

        // Leaves do not block any other block, including themselves
        if(transparent[get(x2, y2, z2)] == 1)
            return false;

        // Non-transparent blocks always block line of sight
        if(!transparent[get(x2, y2, z2)])
            return true;

        // Otherwise, LOS is only blocked by blocks if the same transparency type
        return transparent[get(x2, y2, z2)] == transparent[block[x1][y1][z1]];
    }

    void update() {
        changed = 0;
        fprintf(stderr, "setting chunk as clean\n");

        byte4 vertex[BLOCK_X * BLOCK_Y * BLOCK_Z * 6 * 6];
        int i = 0, merged = 0;
        bool vis;

        for(int x = BLOCK_X - 1; x >= 0; x--) {
            for(int y = 0; y < BLOCK_Y; y++) {
                for(int z = 0; z < BLOCK_Z; z++) {
                    // Line of sight blocked?
                    if(isblocked(x, y, z, x - 1, y, z)) {
                        vis = false;
                        continue;
                    }

                    uint8_t top = block[x][y][z];
                    uint8_t bottom = block[x][y][z];
                    uint8_t side = block[x][y][z];

                    // Grass block has dirt sides and bottom
                    if(top == 3) {
                        bottom = 1;
                        side = 2;
                    // Wood blocks have rings on top and bottom
                    } else if(top == 5) {
                        top = bottom = 12;
                    }

                    // Same block as previous one? Extend it.
                    if(vis && z != 0 && block[x][y][z] == block[x][y][z - 1]) {
                        vertex[i - 5] = byte4(x, y, z + 1, side);
                        vertex[i - 2] = byte4(x, y, z + 1, side);
                        vertex[i - 1] = byte4(x, y + 1, z + 1, side);
                        merged++;
                    // Otherwise, add a new quad.
                    } else {
                        vertex[i++] = byte4(x, y, z, side);
                        vertex[i++] = byte4(x, y, z + 1, side);
                        vertex[i++] = byte4(x, y + 1, z, side);
                        vertex[i++] = byte4(x, y + 1, z, side);
                        vertex[i++] = byte4(x, y, z + 1, side);
                        vertex[i++] = byte4(x, y + 1, z + 1, side);
                    }

                    vis = true;
                }
            }
        }

        // View from positive x

        for(int x = 0; x < BLOCK_X; x++) {
            for(int y = 0; y < BLOCK_Y; y++) {
                for(int z = 0; z < BLOCK_Z; z++) {
                    if(isblocked(x, y, z, x + 1, y, z)) {
                        vis = false;
                        continue;
                    }

                    uint8_t top = block[x][y][z];
                    uint8_t bottom = block[x][y][z];
                    uint8_t side = block[x][y][z];

                    if(top == 3) {
                        bottom = 1;
                        side = 2;
                    } else if(top == 5) {
                        top = bottom = 12;
                    }

                    if(vis && z != 0 && block[x][y][z] == block[x][y][z - 1]) {
                        vertex[i - 4] = byte4(x + 1, y, z + 1, side);
                        vertex[i - 2] = byte4(x + 1, y + 1, z + 1, side);
                        vertex[i - 1] = byte4(x + 1, y, z + 1, side);
                        merged++;
                    } else {
                        vertex[i++] = byte4(x + 1, y, z, side);
                        vertex[i++] = byte4(x + 1, y + 1, z, side);
                        vertex[i++] = byte4(x + 1, y, z + 1, side);
                        vertex[i++] = byte4(x + 1, y + 1, z, side);
                        vertex[i++] = byte4(x + 1, y + 1, z + 1, side);
                        vertex[i++] = byte4(x + 1, y, z + 1, side);
                    }
                    vis = true;
                }
            }
        }

        // View from negative y

        for(int x = 0; x < BLOCK_X; x++) {
            for(int y = BLOCK_Y - 1; y >= 0; y--) {
                for(int z = 0; z < BLOCK_Z; z++) {
                    if(isblocked(x, y, z, x, y - 1, z)) {
                        vis = false;
                        continue;
                    }

                    uint8_t top = block[x][y][z];
                    uint8_t bottom = block[x][y][z];

                    if(top == 3) {
                        bottom = 1;
                    } else if(top == 5) {
                        top = bottom = 12;
                    }

                    if(vis && z != 0 && block[x][y][z] == block[x][y][z - 1]) {
                        vertex[i - 4] = byte4(x, y, z + 1, bottom + 128);
                        vertex[i - 2] = byte4(x + 1, y, z + 1, bottom + 128);
                        vertex[i - 1] = byte4(x, y, z + 1, bottom + 128);
                        merged++;
                    } else {
                        vertex[i++] = byte4(x, y, z, bottom + 128);
                        vertex[i++] = byte4(x + 1, y, z, bottom + 128);
                        vertex[i++] = byte4(x, y, z + 1, bottom + 128);
                        vertex[i++] = byte4(x + 1, y, z, bottom + 128);
                        vertex[i++] = byte4(x + 1, y, z + 1, bottom + 128);
                        vertex[i++] = byte4(x, y, z + 1, bottom + 128);
                    }
                    vis = true;
                }
            }
        }

        // View from positive y

        for(int x = 0; x < BLOCK_X; x++) {
            for(int y = 0; y < BLOCK_Y; y++) {
                for(int z = 0; z < BLOCK_Z; z++) {
                    if(isblocked(x, y, z, x, y + 1, z)) {
                        vis = false;
                        continue;
                    }

                    uint8_t top = block[x][y][z];
                    uint8_t bottom = block[x][y][z];

                    if(top == 3) {
                        bottom = 1;
                    } else if(top == 5) {
                        top = bottom = 12;
                    }

                    if(vis && z != 0 && block[x][y][z] == block[x][y][z - 1]) {
                        vertex[i - 5] = byte4(x, y + 1, z + 1, top + 128);
                        vertex[i - 2] = byte4(x, y + 1, z + 1, top + 128);
                        vertex[i - 1] = byte4(x + 1, y + 1, z + 1, top + 128);
                        merged++;
                    } else {
                        vertex[i++] = byte4(x, y + 1, z, top + 128);
                        vertex[i++] = byte4(x, y + 1, z + 1, top + 128);
                        vertex[i++] = byte4(x + 1, y + 1, z, top + 128);
                        vertex[i++] = byte4(x + 1, y + 1, z, top + 128);
                        vertex[i++] = byte4(x, y + 1, z + 1, top + 128);
                        vertex[i++] = byte4(x + 1, y + 1, z + 1, top + 128);
                    }
                    vis = true;
                }
            }
        }

        // View from negative z

        for(int x = 0; x < BLOCK_X; x++) {
            for(int z = BLOCK_Z - 1; z >= 0; z--) {
                for(int y = 0; y < BLOCK_Y; y++) {
                    if(isblocked(x, y, z, x, y, z - 1)) {
                        vis = false;
                        continue;
                    }

                    uint8_t top = block[x][y][z];
                    uint8_t bottom = block[x][y][z];
                    uint8_t side = block[x][y][z];

                    if(top == 3) {
                        bottom = 1;
                        side = 2;
                    } else if(top == 5) {
                        top = bottom = 12;
                    }

                    if(vis && y != 0 && block[x][y][z] == block[x][y - 1][z]) {
                        vertex[i - 5] = byte4(x, y + 1, z, side);
                        vertex[i - 3] = byte4(x, y + 1, z, side);
                        vertex[i - 2] = byte4(x + 1, y + 1, z, side);
                        merged++;
                    } else {
                        vertex[i++] = byte4(x, y, z, side);
                        vertex[i++] = byte4(x, y + 1, z, side);
                        vertex[i++] = byte4(x + 1, y, z, side);
                        vertex[i++] = byte4(x, y + 1, z, side);
                        vertex[i++] = byte4(x + 1, y + 1, z, side);
                        vertex[i++] = byte4(x + 1, y, z, side);
                    }
                    vis = true;
                }
            }
        }

        // View from positive z

        for(int x = 0; x < BLOCK_X; x++) {
            for(int z = 0; z < BLOCK_Z; z++) {
                for(int y = 0; y < BLOCK_Y; y++) {
                    if(isblocked(x, y, z, x, y, z + 1)) {
                        vis = false;
                        continue;
                    }

                    uint8_t top = block[x][y][z];
                    uint8_t bottom = block[x][y][z];
                    uint8_t side = block[x][y][z];

                    if(top == 3) {
                        bottom = 1;
                        side = 2;
                    } else if(top == 5) {
                        top = bottom = 12;
                    }

                    if(vis && y != 0 && block[x][y][z] == block[x][y - 1][z]) {
                        vertex[i - 4] = byte4(x, y + 1, z + 1, side);
                        vertex[i - 3] = byte4(x, y + 1, z + 1, side);
                        vertex[i - 1] = byte4(x + 1, y + 1, z + 1, side);
                        merged++;
                    } else {
                        vertex[i++] = byte4(x, y, z + 1, side);
                        vertex[i++] = byte4(x + 1, y, z + 1, side);
                        vertex[i++] = byte4(x, y + 1, z + 1, side);
                        vertex[i++] = byte4(x, y + 1, z + 1, side);
                        vertex[i++] = byte4(x + 1, y, z + 1, side);
                        vertex[i++] = byte4(x + 1, y + 1, z + 1, side);
                    }
                    vis = true;
                }
            }
        }

        /*for (int x=0; x<BLOCK_X; x++) {
            for (int y=0; y<BLOCK_Y; y++) {
                for (int z=0; z<BLOCK_Z; z++) {
                    uint8_t type = block[x][y][z];

                    const byte4 frontBottomLeft  = byte4(x,     y,     z,     type);
                    const byte4 backBottomLeft   = byte4(x,     y,     z + 1, type);
                    const byte4 frontTopLeft     = byte4(x,     y + 1, z,     type);
                    const byte4 backTopLeft      = byte4(x,     y + 1, z + 1, type);

                    const byte4 frontBottomRight = byte4(x + 1, y,     z,     type);
                    const byte4 backBottomRight  = byte4(x + 1, y,     z + 1, type);
                    const byte4 frontTopRight    = byte4(x + 1, y + 1, z,     type);
                    const byte4 backTopRight     = byte4(x + 1, y + 1, z + 1, type);

                    if (!type) continue;

                    // left face
                    vertex[i++] = frontBottomLeft;
                    vertex[i++] = backBottomLeft;
                    vertex[i++] = frontTopLeft;
                    vertex[i++] = frontTopLeft;
                    vertex[i++] = backBottomLeft;
                    vertex[i++] = backTopLeft;

                    // right face
                    vertex[i++] = frontBottomRight;
                    vertex[i++] = frontTopRight;
                    vertex[i++] = backBottomRight;
                    vertex[i++] = frontTopRight;
                    vertex[i++] = backTopRight;
                    vertex[i++] = backBottomRight;

                    // front face
                    vertex[i++] = frontBottomRight;
                    vertex[i++] = frontBottomLeft;
                    vertex[i++] = frontTopRight;
                    vertex[i++] = frontTopRight;
                    vertex[i++] = frontBottomLeft;
                    vertex[i++] = frontTopLeft;

                    // back face
                    vertex[i++] = backBottomRight;
                    vertex[i++] = backTopRight;
                    vertex[i++] = backTopLeft;
                    vertex[i++] = backTopLeft;
                    vertex[i++] = backBottomLeft;
                    vertex[i++] = backBottomRight;

                    // top face
                    vertex[i++] = frontTopRight;
                    vertex[i++] = frontTopLeft;
                    vertex[i++] = backTopRight;
                    vertex[i++] = frontTopLeft;
                    vertex[i++] = backTopLeft;
                    vertex[i++] = backTopRight;

                    // bottom face
                    vertex[i++] = frontBottomRight;
                    vertex[i++] = backBottomRight;
                    vertex[i++] = backBottomLeft;
                    vertex[i++] = frontBottomLeft;
                    vertex[i++] = frontBottomRight;
                    vertex[i++] = backBottomLeft;
                }
            }
        }*/

        elements = i;
        exitOnGLError("error before vbo code");
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        fprintf(stderr, "vbo: %d\n", vbo);
        exitOnGLError("error binding vbo: ");
        fprintf(stderr, "loading data...\n");
        glBufferData(GL_ARRAY_BUFFER, i * sizeof(byte4), vertex, GL_STATIC_DRAW);
        fprintf(stderr, "data loaded!\n");
        exitOnGLError("error filling vbo");
        fprintf(stderr, "leaving method\n");
    }

    void render() {
        glEnableVertexAttribArray(0);
        exitOnGLError("error after attrib activation");
        exitOnGLError("error after attrib pointer set");

        if (changed == 1) {
            fprintf(stderr, "chunk is dirty\n");
            update();
            glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, 0, NULL);
        }

        if(!elements) return;

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        exitOnGLError("error after bind");

        glDrawArrays(GL_TRIANGLES, 0, elements);
    }
};

#endif // CHUNK_H_INCLUDED
