#include "Utils.h"

#define WINDOW_TITLE_PREFIX "Playground"

int
  CurrentWidth = 600,
  CurrentHeight = 600,
  WindowHandle = 0;

unsigned frameCount = 0;

GLuint
    projectionMatrixUniformLocation,
    viewMatrixUniformLocation,
    modelMatrixUniformLocation,
    bufferIds[3] = { 0 },
    shaderIds[3] = { 0 };

Matrix projectionMatrix, viewMatrix, modelMatrix;

float cubeRotation = 0;
clock_t lastTime = 0;

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);
void TimerFunction(int);
void IdleFunction(void);
void keyboardFunction(unsigned char, int, int);
void createCube(void);
void destroyCube(void);
void drawCube(void);

int main(int argc, char* argv[])
{
  Initialize(argc, argv);

  glutMainLoop();

  exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[])
{
    GLenum glewInitResult;

    InitWindow(argc, argv);

    glewExperimental = GL_TRUE;
    glewInitResult = glewInit();

    if (glewInitResult != GLEW_OK) {
        fprintf(stderr, "ERROR: %s\n", glewGetErrorString(glewInitResult));
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));

    glGetError();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    exitOnGLError("ERROR: Could not set OpenGL depth testing options");

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    exitOnGLError("ERROR: Could not set OpenGL culling options");

    modelMatrix = IDENTITY_MATRIX;
    projectionMatrix = IDENTITY_MATRIX;
    viewMatrix = IDENTITY_MATRIX;
    translateMatrix(&viewMatrix, 0, 0, -2);

    createCube();
}

void InitWindow(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );

    glutInitWindowSize(CurrentWidth, CurrentHeight);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

    if(WindowHandle < 1) {
        fprintf( stderr, "ERROR: Could not create a new rendering window.\n");
        exit(EXIT_FAILURE);
    }

    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(RenderFunction);
    glutIdleFunc(IdleFunction);
    glutTimerFunc(0, TimerFunction, 0);
    glutCloseFunc(destroyCube);
    glutKeyboardFunc(keyboardFunction);
}

void keyboardFunction(unsigned char key, int x, int y) {
    switch (key) {
        case 'F' :
        case 'f' : {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        }
        case 'L' :
        case 'l' : {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        }
        default:
            break;
    }
}

void ResizeFunction(int Width, int Height)
{
  CurrentWidth = Width;
  CurrentHeight = Height;
  glViewport(0, 0, CurrentWidth, CurrentHeight);

  projectionMatrix = createProjectionMatrix(60, (float)CurrentWidth/CurrentHeight, 1.0f, 100.0f);

  Matrix pvMatrix = multiplyMatrices(&projectionMatrix, &viewMatrix);

  glUseProgram(shaderIds[0]);
  glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, viewMatrix.m);
  glUseProgram(0);
}

void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawCube();

    glutSwapBuffers();
    glutPostRedisplay();

    ++frameCount;
}

void createCube() {
    const Vertex VERTICES[24] = {
        // Lower front left
        { { -.5f, -.5f, -.5f, 1 }, { 0, 0, 1, 1 } }, //0
        { { -.5f, -.5f, -.5f, 1 }, { 1, 0, 0, 1 } },
        { { -.5f, -.5f, -.5f, 1 }, { 0, 1, 0, 1 } },

        // Lower front right
        { {  .5f, -.5f, -.5f, 1 }, { 0, 0, 1, 1 } }, // 3
        { {  .5f, -.5f, -.5f, 1 }, { 1, 0, 0, 1 } },
        { {  .5f, -.5f, -.5f, 1 }, { 1, 1, 0, 1 } },

        // Upper front left
        { { -.5f,  .5f, -.5f, 1 }, { 1, 0, 0, 1 } }, // 6
        { { -.5f,  .5f, -.5f, 1 }, { 0, 1, 0, 1 } },
        { { -.5f,  .5f, -.5f, 1 }, { 0, 1, 1, 1 } },

        // Upper front right
        { {  .5f,  .5f, -.5f, 1 }, { 1, 0, 0, 1 } }, // 9
        { {  .5f,  .5f, -.5f, 1 }, { 0, 1, 1, 1 } },
        { {  .5f,  .5f, -.5f, 1 }, { 1, 1, 0, 1 } },

        // Lower back left
        { { -.5f, -.5f,  .5f, 1 }, { 1, 0, 1, 1 } }, // 12
        { { -.5f, -.5f,  .5f, 1 }, { 0, 1, 0, 1 } },
        { { -.5f, -.5f,  .5f, 1 }, { 0, 0, 1, 1 } },

        // Lower back right
        { {  .5f, -.5f,  .5f, 1 }, { 1, 0, 1, 1 } }, // 15
        { {  .5f, -.5f,  .5f, 1 }, { 1, 1, 0, 1 } },
        { {  .5f, -.5f,  .5f, 1 }, { 0, 0, 1, 1 } },

        // Upper back left
        { { -.5f,  .5f,  .5f, 1 }, { 1, 0, 1, 1 } }, // 18
        { { -.5f,  .5f,  .5f, 1 }, { 0, 1, 1, 1 } },
        { { -.5f,  .5f,  .5f, 1 }, { 0, 1, 0, 1 } },

        // Upper back right
        { {  .5f,  .5f,  .5f, 1 }, { 1, 0, 1, 1 } }, // 21
        { {  .5f,  .5f,  .5f, 1 }, { 0, 1, 1, 1 } },
        { {  .5f,  .5f,  .5f, 1 }, { 1, 1, 0, 1 } }
    };

    const GLuint INDICES[36] = {
        4, 1, 9,  1, 6, 9,  // Red
        0, 3,17, 17,14, 0,  // Blue
        2,13,20, 20, 7, 2,  // Green
        8,22,10,  8,19,22,  // Cyan
       16, 5,11, 16,11,23,  // Yellow
       15,18,12, 15,21,18   // Purple
    };

    shaderIds[0] = glCreateProgram();
    exitOnGLError("ERROR: Could not create the shader program");

    shaderIds[1] = loadShader("SimpleShader.fragment.glsl", GL_FRAGMENT_SHADER);
    shaderIds[2] = loadShader("SimpleShader.vertex.glsl", GL_VERTEX_SHADER);
    glAttachShader(shaderIds[0], shaderIds[1]);
    glAttachShader(shaderIds[0], shaderIds[2]);

    glLinkProgram(shaderIds[0]);
    exitOnGLError("ERROR: Could not link the shader program");

    modelMatrixUniformLocation = glGetUniformLocation(shaderIds[0], "ModelMatrix");
    projectionMatrixUniformLocation = glGetUniformLocation(shaderIds[0], "ProjectionMatrix");
    viewMatrixUniformLocation = glGetUniformLocation(shaderIds[0], "ViewMatrix");
    exitOnGLError("ERROR: Could not get the shader uniform locations");

    glGenBuffers(2, &bufferIds[1]);
    exitOnGLError("ERROR: Could not generate the buffer objects");

    glGenVertexArrays(1, &bufferIds[0]);
    exitOnGLError("ERROR: Could not generate the VAO");
    glBindVertexArray(bufferIds[0]);
    exitOnGLError("ERROR: Could not bind the VAO");

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    exitOnGLError("ERROR: Could not enable the vertex attributes");

    glBindBuffer(GL_ARRAY_BUFFER, bufferIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    exitOnGLError("ERROR: Could not bind the VBO to the VAO");

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), NULL);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)sizeof(VERTICES[0].Position));
    exitOnGLError("ERROR: Could not set the VAO attributes");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
    exitOnGLError("ERROR: Could not bind the IBO to the VAO");

    glBindVertexArray(0);
}

void destroyCube(void) {
    glDetachShader(shaderIds[0], shaderIds[1]);
    glDetachShader(shaderIds[0], shaderIds[2]);
    glDeleteShader(shaderIds[1]);
    glDeleteShader(shaderIds[2]);
    glDeleteProgram(shaderIds[0]);
    exitOnGLError("ERROR: Could not destroy the shaders");

    glDeleteBuffers(2, &bufferIds[1]);
    glDeleteVertexArrays(1, &bufferIds[0]);
    exitOnGLError("ERROR: Could not destroy the buffer objects");
}

void drawCube(void) {
    clock_t now = clock();
    if (lastTime == 0) lastTime = now;

    cubeRotation += 1000.0f * ((float)(now - lastTime) / CLOCKS_PER_SEC);
    float cubeAngle = degreesToRadians(cubeRotation);
    lastTime = now;

    modelMatrix = IDENTITY_MATRIX;
    rotateAboutY(&modelMatrix, cubeAngle);
    rotateAboutX(&modelMatrix, cubeAngle);

    glUseProgram(shaderIds[0]);
    exitOnGLError("ERROR: Could not use the shader program");

    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, modelMatrix.m);
    glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, viewMatrix.m);
    exitOnGLError("ERROR: Could not set the shader uniforms");

    glBindVertexArray(bufferIds[0]);
    exitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
    exitOnGLError("ERROR: Could not draw the cube");

    glBindVertexArray(0);
    glUseProgram(0);
}

void IdleFunction(void) {
    glutPostRedisplay();
}

void TimerFunction(int value) {
    if (0 != value) {
        char *tempString = (char*) malloc(512 + strlen(WINDOW_TITLE_PREFIX));

        sprintf(tempString, "%s: %d frames per second @ %d x %d", WINDOW_TITLE_PREFIX, frameCount * 4, CurrentWidth, CurrentHeight);

        glutSetWindowTitle(tempString);
        free(tempString);
    }

    frameCount = 0;
    glutTimerFunc(250, TimerFunction, 1);
}
