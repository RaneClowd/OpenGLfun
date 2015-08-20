#include "Cube.h"

static const GLuint numCubeIndices = 36;
static GLuint voaID = -1;

static GLuint vertexBufferID = -1;
static GLuint indexBufferID = -1;

Cube::Cube() { }

void Cube::freeShapeResources() {
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteBuffers(1, &indexBufferID);
    glDeleteVertexArrays(1, &voaID);

    printf("Cube shape data released\n");
}

void Cube::defineData() {
    Vertex VERTICES[numCubeIndices];

    // Lower front left
    VERTICES[ 0] = { { -.5f, -.5f, -.5f }, { 0,-1, 0 } };
    VERTICES[ 1] = { { -.5f, -.5f, -.5f }, { 0, 0,-1 } };
    VERTICES[ 2] = { { -.5f, -.5f, -.5f }, {-1, 0, 0 } };

    // Lower front right
    VERTICES[ 3] = { {  .5f, -.5f, -.5f }, { 0,-1, 0 } };
    VERTICES[ 4] = { {  .5f, -.5f, -.5f }, { 0, 0,-1 } };
    VERTICES[ 5] = { {  .5f, -.5f, -.5f }, { 1, 0, 0 } };

    // Upper front left
    VERTICES[ 6] = { { -.5f,  .5f, -.5f }, { 0, 0,-1 } };
    VERTICES[ 7] = { { -.5f,  .5f, -.5f }, {-1, 0, 0 } };
    VERTICES[ 8] = { { -.5f,  .5f, -.5f }, { 0, 1, 0 } };

    // Upper front right
    VERTICES[ 9] = { {  .5f,  .5f, -.5f }, { 0, 0,-1 } };
    VERTICES[10] = { {  .5f,  .5f, -.5f }, { 0, 1, 0 } };
    VERTICES[11] = { {  .5f,  .5f, -.5f }, { 1, 0, 0 } };

    // Lower back left
    VERTICES[12] = { { -.5f, -.5f,  .5f }, { 0, 0, 1 } };
    VERTICES[13] = { { -.5f, -.5f,  .5f }, {-1, 0, 0 } };
    VERTICES[14] = { { -.5f, -.5f,  .5f }, { 0,-1, 0 } };

    // Lower back right
    VERTICES[15] = { {  .5f, -.5f,  .5f }, { 0, 0, 1 } };
    VERTICES[16] = { {  .5f, -.5f,  .5f }, { 1, 0, 0 } };
    VERTICES[17] = { {  .5f, -.5f,  .5f }, { 0,-1, 0 } };

    // Upper back left
    VERTICES[18] = { { -.5f,  .5f,  .5f }, { 0, 0, 1 } };
    VERTICES[19] = { { -.5f,  .5f,  .5f }, { 0, 1, 0 } };
    VERTICES[20] = { { -.5f,  .5f,  .5f }, {-1, 0, 0 } };

    // Upper back right
    VERTICES[21] = { {  .5f,  .5f,  .5f }, { 0, 0, 1 } };
    VERTICES[22] = { {  .5f,  .5f,  .5f }, { 0, 1, 0 } };
    VERTICES[23] = { {  .5f,  .5f,  .5f }, { 1, 0, 0 } };

    const GLuint INDICES[numCubeIndices] = {
        4, 1, 9,  1, 6, 9,      // front
        0, 3,17, 17,14, 0,      // bottom
        2,13,20, 20, 7, 2,      // left
        8,22,10,  8,19,22,      // top
       16, 5,11, 16,11,23,      // right
       15,18,12, 15,21,18,      // back
    };

    glGenBuffers(1, &vertexBufferID);
    glGenBuffers(1, &indexBufferID);

    glGenVertexArrays(1, &voaID);
    glBindVertexArray(voaID);


    // Set up vertex data
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)sizeof(VERTICES[0].Position));


    // Set up index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

GLuint Cube::getVertexArrayObjectID() {
    return voaID;
}

GLuint Cube::getNumDataIndices() {
    return numCubeIndices;
}
