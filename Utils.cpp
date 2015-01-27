#include "Utils.h"
#include <unistd.h>

const glm::mat4 IDENTITY_MATRIX = glm::mat4(
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1);

float degreesToRadians(float degrees)
{
  return degrees * (float)(PI / 180);
}

void verticesForCircle(Vertex *vertexArray, int vertexStart, int numVertices) {
    float radianIncrement = numVertices / 2.0f * PI;
    float radians = 0.0f;
    for (int i = 0; i < numVertices; i++) {
        vertexArray[i+vertexStart] = { { cos(radians), 0.0f, sin(radians), 1 }, {1,0,0,1}};
        radians += radianIncrement;
    }
}

Vertex* verticesForSphere(int& count, float radius, int slices, int stacks, glm::vec4 color) {
    count = stacks * slices;

    Vertex *vertexArray = new Vertex[count];
    int vertexIndex = 0;

    for (int stack = 0; stack < stacks; ++stack) {
        for (int slice = 0; slice < slices; ++slice) {
            float y = -cos(PI * stack / stacks);
            /* for better performance, use y = 2.0 * stack / STACKS - 1.0 */

            float r = sqrt(1 - pow(y, 2));
            float x = r * sin(2.0 * PI * slice / slices);
            float z = r * cos(2.0 * PI * slice / slices);


            vertexArray[vertexIndex].Position = glm::vec4(x*radius, y*radius, z*radius, 1);
            vertexArray[vertexIndex].Color = color;

            vertexIndex += 1;
        }
    }
}



void exitOnGLError(const char* error_message)
{
  const GLenum ErrorValue = glGetError();

  if (ErrorValue != GL_NO_ERROR)
  {
    fprintf(stderr, "%s: %s\n", error_message, glewGetErrorString(ErrorValue));
    exit(EXIT_FAILURE);
  }
}

GLuint loadShader(const char* filename, GLenum shader_type)
{
    GLuint shader_id = 0;
    FILE* file;
    long file_size = -1;
    char* glsl_source;

    file = fopen(filename, "rb");

    if (file == NULL) {
        fprintf(stderr, "ERROR: Could not get file handle.\n");
        perror("Error:");
        return 0;
    }

    if (0 == fseek(file, 0, SEEK_END) && -1 != (file_size = ftell(file))) {
        rewind(file);

        if (NULL != (glsl_source = (char*)malloc(file_size + 1))) {
            if (file_size == (long)fread(glsl_source, sizeof(char), file_size, file)) {
                glsl_source[file_size] = '\0';

                if (0 != (shader_id = glCreateShader(shader_type))) {
                    glShaderSource(shader_id, 1, (const GLchar **)&glsl_source, NULL);
                    glCompileShader(shader_id);
                    exitOnGLError("Could not compile a shader");
                } else {
                    fprintf(stderr, "ERROR: Could not create a shader.\n");
                }
            } else {
                fprintf(stderr, "ERROR: Could not read file %s\n", filename);
            }

            free(glsl_source);
        } else {
            fprintf(stderr, "ERROR: Could not allocate %i bytes.\n", (int)file_size);
        }

        fclose(file);
    } else {
        if (NULL != file) fclose(file);
        fprintf(stderr, "ERROR: Could not open file %s\n", filename);
    }

    return shader_id;
}

void printWorkingDirectory(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        fprintf(stdout, "\nCurrent working dir: %s\n\n", cwd);
    } else {
        perror("getcwd() error");
    }
}
