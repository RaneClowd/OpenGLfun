#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const double PI = 3.14159265358979323846;

typedef struct Vertex
{
  glm::vec4 Position;
  glm::vec4 Color;
} Vertex;

typedef enum {
    NONE = 0,
    FORWARD = 1,
    BACKWARD = 1<<1,
    LEFT = 1<<2,
    RIGHT = 1<<3
} Direction;

extern const glm::mat4 IDENTITY_MATRIX;

float degreesToRadians(float degrees);

Vertex* verticesForSphere(int& count, float radius, int slices, int stacks, glm::vec4 color);

void exitOnGLError(const char* error_message);
GLuint loadShader(const char* filename, GLenum shader_type);

// For debugging help
void printWorkingDirectory(void);

#endif
