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

/*void verticesForCircle(Vertex *vertexArray, int vertexStart, int numVertices) {
    float radianIncrement = numVertices / 2.0f * PI;
    float radians = 0.0f;
    for (int i = 0; i < numVertices; i++) {
        vertexArray[i+vertexStart] = { { cos(radians), 0.0f, sin(radians), 1 }, {1,0,0,1}};
        radians += radianIncrement;
    }
}*/

void verticesForSphere(int granularity, Vertex** vertexArray, int* indexArray) {
    float t = (1.0 + sqrt(5.0)) / 2.0;

    int numVertices = 4 * 3; // 3 rectangles to make icosahedron
    Vertex* vArray = (Vertex*)malloc(sizeof(Vertex) * numVertices);

    for (int index = 0; index < numVertices; index++) {
        vArray[index].Color = glm::vec4(0, 1, 1, 1);
    }

    vArray[ 0].Position = glm::vec4(-1,  t,  0, 1);
    vArray[ 1].Position = glm::vec4( 1,  t,  0, 1);
    vArray[ 2].Position = glm::vec4(-1, -t,  0, 1);
    vArray[ 3].Position = glm::vec4( 1, -t,  0, 1);

    vArray[ 4].Position = glm::vec4( 0, -1,  t, 1);
    vArray[ 5].Position = glm::vec4( 0,  1,  t, 1);
    vArray[ 6].Position = glm::vec4( 0, -1, -t, 1);
    vArray[ 7].Position = glm::vec4( 0,  1, -t, 1);

    vArray[ 8].Position = glm::vec4( t,  0, -1, 1);
    vArray[ 9].Position = glm::vec4( t,  0,  1, 1);
    vArray[10].Position = glm::vec4(-t,  0, -1, 1);
    vArray[11].Position = glm::vec4(-t,  0,  1, 1);


    indexArray[ 0]= 0; indexArray[ 1]=11, indexArray[ 2]= 5;
    indexArray[ 3]= 0; indexArray[ 4]= 5, indexArray[ 5]= 1;
    indexArray[ 6]= 0; indexArray[ 7]= 1, indexArray[ 8]= 7;
    indexArray[ 9]= 0; indexArray[10]= 7, indexArray[11]=10;
    indexArray[12]= 0; indexArray[13]=10, indexArray[14]=11;

    indexArray[15]= 1; indexArray[16]= 5, indexArray[17]= 9;
    indexArray[18]= 5; indexArray[19]=11, indexArray[20]= 4;
    indexArray[21]=11; indexArray[22]=10, indexArray[23]= 2;
    indexArray[24]=10; indexArray[25]= 7, indexArray[26]= 6;
    indexArray[27]= 7; indexArray[28]= 1, indexArray[29]= 8;

    indexArray[30]= 3; indexArray[31]= 9, indexArray[32]= 4;
    indexArray[33]= 3; indexArray[34]= 4, indexArray[35]= 2;
    indexArray[36]= 3; indexArray[37]= 2, indexArray[38]= 6;
    indexArray[39]= 3; indexArray[40]= 6, indexArray[41]= 8;
    indexArray[42]= 3; indexArray[43]= 8, indexArray[44]= 9;

    indexArray[45]= 4; indexArray[46]= 9, indexArray[47]= 5;
    indexArray[48]= 2; indexArray[49]= 4, indexArray[50]=11;
    indexArray[51]= 6; indexArray[52]= 2, indexArray[53]=10;
    indexArray[54]= 8; indexArray[55]= 6, indexArray[56]= 7;
    indexArray[57]= 9; indexArray[58]= 8, indexArray[59]= 1;

    *vertexArray = vArray;

    /*count = stacks * slices;

    Vertex *vertexArray = new Vertex[count];
    int vertexIndex = 0;

    for (int stack = 0; stack < stacks; ++stack) {
        for (int slice = 0; slice < slices; ++slice) {
            float y = -cos(PI * stack / stacks);

            float r = sqrt(1 - pow(y, 2));
            float x = r * sin(2.0 * PI * slice / slices);
            float z = r * cos(2.0 * PI * slice / slices);


            vertexArray[vertexIndex].Position = glm::vec4(x*radius, y*radius, z*radius, 1);
            vertexArray[vertexIndex].Color = color;

            vertexIndex += 1;
        }
    }*/
}



void exitOnGLError(const char* error_message)
{
  const GLenum ErrorValue = glGetError();

  if (ErrorValue != GL_NO_ERROR)
  {
    fprintf(stderr, "%s: %s (%d)\n", error_message, glewGetErrorString(ErrorValue), ErrorValue);
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
