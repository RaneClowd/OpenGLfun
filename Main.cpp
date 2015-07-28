#include "GLHelpers/GLShader.h"

#include "Utils.h"
#include "Player.h"

#include "Chunk.h"
#include "texture.c"

int winWidth = 800, winHeight = 800;

unsigned frameCount = 0;

GLuint
    mvpUniform,
    textureUniform,
    bufferIds[3] = { 0 },
    shaderIds[3] = { 0 },
    texture;

SDL_Window *window;
SDL_GLContext glContext;

GLShader vertexShader, fragmentShader;

glm::mat4 viewMatrix, projectionMatrix;

float cubeRotation = 0;
clock_t lastTime = 0;

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

    glClearColor(0.9f, 0.4f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    exitOnGLError("ERROR: Could not set OpenGL depth testing options");

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    exitOnGLError("ERROR: Could not set OpenGL culling options");
}

void initShaders() {
    shaderIds[0] = glCreateProgram();
    exitOnGLError("ERROR: Could not create the shader program");

    vertexShader.loadShader("SimpleShader.vertex.glsl", GL_VERTEX_SHADER);
    fragmentShader.loadShader("SimpleShader.fragment.glsl", GL_FRAGMENT_SHADER);
    glAttachShader(shaderIds[0], vertexShader.getShaderID());
    glAttachShader(shaderIds[0], fragmentShader.getShaderID());

    glLinkProgram(shaderIds[0]);
    exitOnGLError("ERROR: Could not link the shader program");

    glUseProgram(shaderIds[0]);
    exitOnGLError("ERROR: Problem using compiled program");
    mvpUniform = glGetUniformLocation(shaderIds[0], "mvp");
    exitOnGLError("ERROR: Could not get the shader uniform locations");

    glGenVertexArrays(1, &bufferIds[0]);
    glBindVertexArray(bufferIds[0]);
}

void initTexture() {
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures.width, textures.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures.pixel_data);

    textureUniform = glGetUniformLocation(shaderIds[0], "texture");
    glUniform1i(textureUniform, /*GL_TEXTURE*/0);
}

void freeResources(void) {
    glDetachShader(shaderIds[0], shaderIds[1]);
    glDetachShader(shaderIds[0], shaderIds[2]);
    glDeleteShader(shaderIds[1]);
    glDeleteShader(shaderIds[2]);
    glDeleteProgram(shaderIds[0]);
    exitOnGLError("ERROR: Could not destroy the shaders");

    glDeleteBuffers(2, &bufferIds[1]);
    glDeleteVertexArrays(1, &bufferIds[0]);
    exitOnGLError("ERROR: Could not destroy the buffer objects");

    glDeleteTextures(1, &texture);
    exitOnGLError("ERROR: Could not destroy texture");
}

void clearGraphics(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render(float timeLapsed, Chunk *scene) {
    glm::mat4 mvp = projectionMatrix * viewMatrix;
    glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

    clearGraphics();

    exitOnGLError("error before scene render");
    scene->render();
    exitOnGLError("error after scene render");

    SDL_GL_SwapWindow(window);
}

int main(int argc, char* argv[]) {
    printWorkingDirectory();

    initSDLWithOpenGL();
    initGlew();
    initShaders();
    initTexture();

    projectionMatrix = glm::perspective(45.0f, ((float)winWidth)/winHeight, .01f, 1000.0f);

    Chunk scene;
    for (int x=0; x<8; x++) {
        scene.set(x, 0, 0, 1);
        scene.set(0, x, 0, 1);
        scene.set(0, 0, x, 1);
    }
    scene.set(15, 1, 15, 3);
    scene.set(15, -5, 15, 3);
    scene.set(15, 6, 15, 3);
    exitOnGLError("error after init");

    initPlayerInput();
    exitOnGLError("error before while");
    while (!userQuit()) {

        exitOnGLError("error before time check");
        clock_t now = clock();
        if (lastTime == 0) lastTime = now;
        float timeLapsed = ((float)(now - lastTime) / CLOCKS_PER_SEC);
        lastTime = now;

        exitOnGLError("error before player input");
        checkForPlayerInput();
        viewMatrix = updatePlayerView(timeLapsed);

        SDL_WarpMouseInWindow(window, winWidth/2, winHeight/2);
        exitOnGLError("error before render");
        render(timeLapsed, &scene);

        exitOnGLError("error after render");

        SDL_Delay(20 - (timeLapsed * 1000));
    }

    freeResources();
    scene.~Chunk();

    SDL_GL_DeleteContext(glContext);
    SDL_Quit();
    exit(EXIT_SUCCESS);
}
