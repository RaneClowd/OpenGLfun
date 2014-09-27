#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

static const double PI = 3.14159265358979323846;

typedef struct Vertex
{
  float Position[4];
  float Color[4];
} Vertex;

typedef enum {
    NONE = 0,
    FORWARD = 1,
    BACKWARD = 1<<1,
    LEFT = 1<<2,
    RIGHT = 1<<3
} Direction;

typedef struct Matrix
{
  float m[16];
} Matrix;

extern const Matrix IDENTITY_MATRIX;

float cotangent(float angle);
float degreesToRadians(float degrees);
float radiansToDegrees(float radians);

Matrix multiplyMatrices(const Matrix* m1, const Matrix* m2);
void rotateAboutX(Matrix* m, float angle);
void rotateAboutY(Matrix* m, float angle);
void rotateAboutZ(Matrix* m, float angle);
void scaleMatrix(Matrix* m, float x, float y, float z);
void translateMatrix(Matrix* m, float x, float y, float z);

Matrix createProjectionMatrix(
  float fovy,
  float aspect_ratio,
  float near_plane,
  float far_plane
);

void exitOnGLError(const char* error_message);
GLuint loadShader(const char* filename, GLenum shader_type);
void printWorkingDirectory(void);

#endif
