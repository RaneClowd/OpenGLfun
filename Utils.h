#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glew.h> // libglew-dev
#include <SDL2/SDL.h> // libsdl2-dev

#include <glm/vec3.hpp> // libglm-dev
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <iterator>

static const double PI = 3.14159265358979323846;

typedef struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
} Vertex;

typedef struct Light
{
    glm::vec3 position;
    glm::vec3 intensities;
} Light;

typedef enum {
    NONE = 0,
    FORWARD = 1,
    BACKWARD = 1<<1,
    LEFT = 1<<2,
    RIGHT = 1<<3
} Direction;

extern const glm::mat4 IDENTITY_MATRIX;

float degreesToRadians(float degrees);

void verticesForSphere(int granularity, Vertex** vertexArray, int* indexArray);

void exitOnGLError(const char* error_message);

// For debugging help
void printWorkingDirectory(void);

#endif
