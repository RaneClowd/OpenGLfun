#include "Utils.h"
#include "Player.h"

#define WINDOW_TITLE_PREFIX "Playground"

int winWidth = 600, winHeight = 600;

unsigned frameCount = 0;

GLuint
    projectionMatrixUniformLocation,
    viewMatrixUniformLocation,
    modelMatrixUniformLocation,
    bufferIds[3] = { 0 },
    shaderIds[3] = { 0 };

SDL_Window *window;
SDL_GLContext glContext;

Matrix projectionMatrix, viewMatrix, modelMatrix;

float cubeRotation = 0;
clock_t lastTime = 0;

void initSDLWithOpenGL(void);
void initGlew(void);
void initShapeData(void);
void setUpProjectionMatrix(void);
void createCube(void);

void RenderFunction(void);
void TimerFunction(int);
void IdleFunction(void);
void keyboardFunction(unsigned char, int, int);
void mouseMove(int, int);
void destroyCube(void);
void drawCube(void);

int main(int argc, char* argv[]) {
    printWorkingDirectory();

    initSDLWithOpenGL();
    initGlew();
    initShapeData();

    setUpProjectionMatrix();

    initPlayerInput();
    while (!userQuit()) {
        checkForPlayerInput();
        viewMatrix = updatePlayerView();

        SDL_WarpMouseInWindow(window, winWidth/2, winHeight/2);
        drawCube();
    }


    destroyCube();
    SDL_GL_DeleteContext(glContext);
    SDL_Quit();
    exit(EXIT_SUCCESS);
}

void initSDLWithOpenGL (void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    glContext = SDL_GL_CreateContext(window);
    if (glContext == NULL) {
        printf("Could not create GL context! SDL_Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_GL_MakeCurrent(window, glContext);
}

void initGlew(void)
{
    GLenum glewInitResult;

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
}

void setUpProjectionMatrix() {
    glViewport(0, 0, winWidth, winHeight);

    projectionMatrix = createProjectionMatrix(60, (float)winWidth/winHeight, 0.4f, 100.0f);

    glUseProgram(shaderIds[0]);
    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, projectionMatrix.m);
    glUseProgram(0);
}

void initShapeData(void) {
    modelMatrix = IDENTITY_MATRIX;

    createCube();
}

void keyboardFunction(unsigned char key, int x, int y) {
    /*switch (key) {
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

        case '<':
        case ',': {
            translateMatrix(&viewMatrix, 0, 0, -1);
            break;
        }

        case 'O':
        case 'o': {
            translateMatrix(&viewMatrix, 0, 0, 1);
            break;
        }

        case 27: {
            glutDestroyWindow(WindowHandle);
            exit(0);
            break;
        }

        default:
            break;
    }*/
}

void mouseMove(int x, int y) {
    /*fprintf(stderr, "x: %d, y: %d", x, y);
    glutWarpPointer(CurrentWidth / 2, CurrentHeight / 2);*/
}

void RenderFunction(void)
{
    /*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawCube();

    glutSwapBuffers();
    glutPostRedisplay();

    ++frameCount;*/
}

void createCube() {
    const Vertex VERTICES[36] = {
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
        { { -.5f,  .5f, -.5f, 1 }, { 0, 0, 0, 1 } },

        // Upper front right
        { {  .5f,  .5f, -.5f, 1 }, { 1, 0, 0, 1 } }, // 9
        { {  .5f,  .5f, -.5f, 1 }, { 0, 0, 0, 1 } },
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
        { { -.5f,  .5f,  .5f, 1 }, { 0, 0, 0, 1 } },
        { { -.5f,  .5f,  .5f, 1 }, { 0, 1, 0, 1 } },

        // Upper back right
        { {  .5f,  .5f,  .5f, 1 }, { 1, 0, 1, 1 } }, // 21
        { {  .5f,  .5f,  .5f, 1 }, { 0, 0, 0, 1 } },
        { {  .5f,  .5f,  .5f, 1 }, { 1, 1, 0, 1 } },

        // Ground
        { {  -.5f, .0f, -.5f, 1 }, { 0, .6, 0, 1 } }, // 24
        { {   .5f, .0f, -.5f, 1 }, { 0, .6, 0, 1 } },
        { {   .5f, .0f,  .5f, 1 }, { 0, .6, 0, 1 } },
        { {  -.5f, .0f,  .5f, 1 }, { 0, .6, 0, 1 } },

        // Sky box
        { { -.5f,  .0f, -.5f, 1 }, { 0, .7, .9, 1 } }, // Lower back left         28
        { {  .5f,  .0f, -.5f, 1 }, { 0, .7, .9, 1 } }, // Lower back right
        { { -.5f,  .5f, -.5f, 1 }, { 0, .7, .9, 1 } }, // Upper back left         30
        { {  .5f,  .5f, -.5f, 1 }, { 0, .7, .9, 1 } }, // Upper back right
        { { -.5f,  .0f,  .5f, 1 }, { 0, .7, .9, 1 } }, // Lower front left        32
        { {  .5f,  .0f,  .5f, 1 }, { 0, .7, .9, 1 } }, // Lower front right
        { { -.5f,  .5f,  .5f, 1 }, { 0, .7, .9, 1 } }, // Upper front left        34
        { {  .5f,  .5f,  .5f, 1 }, { 0, .7, .9, 1 } }, // Upper front right
    };

    const GLuint INDICES[72] = {
        4, 1, 9,  1, 6, 9,  // Red
        0, 3,17, 17,14, 0,  // Blue
        2,13,20, 20, 7, 2,  // Green
        8,22,10,  8,19,22,  // Cyan
       16, 5,11, 16,11,23,  // Yellow
       15,18,12, 15,21,18,  // Purple

       25,24,27, 27,26,25,  // ground

       34,32,30, 32,28,30,
       31,28,29, 31,30,28,
       35,33,32, 32,34,35,
       29,35,31, 29,33,35,
       34,30,35, 30,31,35
    };

    shaderIds[0] = glCreateProgram();
    exitOnGLError("ERROR: Could not create the shader program");

    shaderIds[1] = loadShader("SimpleShader.fragment.glsl", GL_FRAGMENT_SHADER);
    shaderIds[2] = loadShader("SimpleShader.vertex.glsl", GL_VERTEX_SHADER);
    glAttachShader(shaderIds[0], shaderIds[1]);
    glAttachShader(shaderIds[0], shaderIds[2]);

    glLinkProgram(shaderIds[0]);
    exitOnGLError("ERROR: Could not link the shader program");

    glUseProgram(shaderIds[0]);
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

void clearToBlack(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void drawScene(void) {
    Matrix groundModel = IDENTITY_MATRIX;
    scaleMatrix(&groundModel, 30, 1, 30);
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, groundModel.m);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(36 * sizeof(GLuint)));

    scaleMatrix(&groundModel, 1, 30, 1);
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, groundModel.m);

    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void*)(42 * sizeof(GLuint)));
}

void drawCube(void) {
    clearToBlack();

    clock_t now = clock();
    if (lastTime == 0) lastTime = now;

    cubeRotation += 1000.0f * ((float)(now - lastTime) / CLOCKS_PER_SEC);
    float cubeAngle = degreesToRadians(cubeRotation);
    lastTime = now;

    modelMatrix = IDENTITY_MATRIX;
    rotateAboutY(&modelMatrix, cubeAngle);
    rotateAboutX(&modelMatrix, cubeAngle);
    translateMatrix(&modelMatrix, 0, 1, 0);

    glUseProgram(shaderIds[0]);
    exitOnGLError("ERROR: Could not use the shader program");

    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, modelMatrix.m);
    glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, viewMatrix.m);
    exitOnGLError("ERROR: Could not set the shader uniforms");

    glBindVertexArray(bufferIds[0]);
    exitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
    exitOnGLError("ERROR: Could not draw the cube");

    drawScene();

    glBindVertexArray(0);
    glUseProgram(0);

    SDL_GL_SwapWindow(window);
    SDL_Delay(20);
}

void IdleFunction(void) {
    //glutPostRedisplay();
}

void TimerFunction(int value) {
    /*if (0 != value) {
        char *tempString = (char*) malloc(512 + strlen(WINDOW_TITLE_PREFIX));

        sprintf(tempString, "%s: %d frames per second @ %d x %d", WINDOW_TITLE_PREFIX, frameCount * 4, CurrentWidth, CurrentHeight);

        glutSetWindowTitle(tempString);
        free(tempString);
    }

    frameCount = 0;
    glutTimerFunc(250, TimerFunction, 1);*/
}
