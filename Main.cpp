#include "GLHelpers/GLShader.h"

#include "Utils.h"
#include "Player.h"
#include "Cube.h"
#include "GLProgram.h"


int winWidth = 800, winHeight = 800;
Uint32 msPerFrame = 17; // roughly 60 frames every 1000 milliseconds

GLint sdlWindowFrameBufferID = 0;

SDL_Window *window;
SDL_GLContext glContext;

Light myLight;
Cube lightCube;

Cube bigCube;
Cube floorCube;

const int numCubes = 100;
Cube myCubes[numCubes];

GLProgram shaderProgram;

glm::mat4 viewMatrix, projectionMatrix;

float cubeRotation = .001;
Uint32 lastTime = 0;

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

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &sdlWindowFrameBufferID);
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

    glClearColor(0.0f, 0.2f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    exitOnGLError("ERROR: Could not set OpenGL depth testing options");

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    exitOnGLError("ERROR: Could not set OpenGL culling options");
}

void initShaders() {
    shaderProgram.loadShader("SimpleShader.vertex.glsl", GL_VERTEX_SHADER);
    shaderProgram.loadShader("SimpleShader.fragment.glsl", GL_FRAGMENT_SHADER);
    shaderProgram.linkAndUse();
}

void freeResources(void) {
    shaderProgram.~GLProgram();

    Cube::freeGLResources();
    exitOnGLError("ERROR: Could not destroy the buffer objects");
}

void clearGraphics(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render(float timeLapsed) {
    glBindFramebuffer(GL_FRAMEBUFFER, sdlWindowFrameBufferID);

    glm::mat4 vpMat = projectionMatrix * viewMatrix;
    Cube::viewProjectionMatrix = vpMat;

    clearGraphics();

    exitOnGLError("error before scene render");
    for (int i = 0; i < numCubes; i++) {
        myCubes[i].drawToGL();
    }
    lightCube.drawToGL();
    bigCube.drawToGL();
    floorCube.drawToGL();
    exitOnGLError("error after scene render");

    SDL_GL_SwapWindow(window);
}




GLuint shadowMapTexID;

void initShadowMapFrameBufferAndTexture(void) {
    GLuint frameBufferID = 0;
    glGenFramebuffers(1, &frameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

    glGenTextures(1, &shadowMapTexID);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, winWidth, winHeight, 0, GL_DEPTH_COMPONENT,
                    GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexID, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    exitOnGLError("problem with frame buffer configuration");
    GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fbStatus != GL_FRAMEBUFFER_COMPLETE) {
        printf("FRAME BUFFER ERROR: status: 0x%x\n", fbStatus);
        exit(EXIT_FAILURE);
    }
}

void bindTextureForReading() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexID);
}




float lowestDelay = 20;
int printCount = 0;
int main(int argc, char* argv[]) {
    printWorkingDirectory();

    initSDLWithOpenGL();
    initGlew();
    initShaders();

    printf("getting cube ready\n");
    Cube::initGLResources();
    printf("cube ready\n");

    projectionMatrix = glm::perspective(45.0f, ((float)winWidth)/winHeight, .01f, 1000.0f);

    exitOnGLError("error after init");

    initPlayerInput();
    exitOnGLError("error before while");

    Light myLight;
    lightCube.shaderProgram = &shaderProgram;

    bigCube.shaderProgram = &shaderProgram;
    floorCube.shaderProgram = &shaderProgram;


    int sqrtCubes = sqrt(numCubes);
    for (int i = 0; i < sqrtCubes; i++) {
        for (int j = 0; j < sqrtCubes; j++) {
            myCubes[i*sqrtCubes + j].translate(glm::vec3(i*2, 0, j*2));
            myCubes[i*sqrtCubes + j].shaderProgram = &shaderProgram;
        }
    }

    myLight.position = glm::vec3(0, 2, 0);
    myLight.intensities = glm::vec3(1, 1, 1);
    lightCube.translate(glm::vec3(0,2,0));
    lightCube.scale(glm::vec3(.1, .1, .1));

    bigCube.scale(glm::vec3(.3, 15, 30));
    bigCube.translate(glm::vec3(-5, 0, 0));
    bigCube.color = glm::vec3(0, 1, 0);

    floorCube.scale(glm::vec3(40, 1, 40));
    floorCube.translate(glm::vec3(0, -3, 0));
    floorCube.color = glm::vec3(0, 0, 1);

    shaderProgram.loadToUniform("light.position", myLight.position);
    shaderProgram.loadToUniform("light.intensities", myLight.intensities);
    shaderProgram.loadToUniformf("light.attenuation", .2);
    shaderProgram.loadToUniformf("light.ambientCoefficient", .005);

    shaderProgram.loadToUniformf("shininess", 80);
    shaderProgram.loadToUniform("specularColor", glm::vec3(1));

    printf("starting run loop\n");
    while (!userQuit()) {

        exitOnGLError("error before time check");
        Uint32 now = SDL_GetTicks();

        if (lastTime == 0) lastTime = now;
        Uint32 msLapsed = now - lastTime;
        lastTime = now;

        for (int i = 0; i < numCubes; i++) {
            myCubes[i].rotate(glm::vec3(cubeRotation*msLapsed));
        }

        exitOnGLError("error before player input proccessed");
        checkForPlayerInput();
        viewMatrix = updatePlayerView(msLapsed);
        shaderProgram.loadToUniform("cameraPosition", getPlayerLocation());

        SDL_WarpMouseInWindow(window, winWidth/2, winHeight/2);

        exitOnGLError("error before render");
        render(msLapsed);
        exitOnGLError("error after render");

        Uint32 msUsed = SDL_GetTicks() - now;
        if (msUsed < 0) {
            printf("ERROR: frame rate dropping");
            msUsed = 0;
        }

        SDL_Delay(msPerFrame - msUsed);
    }

    freeResources();

    SDL_GL_DeleteContext(glContext);
    SDL_Quit();
    exit(EXIT_SUCCESS);
}
