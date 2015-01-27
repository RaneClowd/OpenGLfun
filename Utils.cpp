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

void printWorkingDirectory(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        fprintf(stdout, "\nCurrent working dir: %s\n\n", cwd);
    } else {
        perror("getcwd() error");
    }
}
