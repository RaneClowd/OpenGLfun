#include "GLHelpers/GLShader.h"

#include "Utils.h"
#include "Player.h"
#include "Cube.h"


int winWidth = 800, winHeight = 800;
Uint32 msPerFrame = 17; // roughly 60 frames every 1000 milliseconds

GLuint
    mvpUniform,
    textureUniform,
    bufferIds[3] = { 0 },
    shaderIds[3] = { 0 },
    texture;

SDL_Window *window;
SDL_GLContext glContext;

Light myLight;
Cube lightCube;

Cube bigCube;
Cube floorCube;

const int numCubes = 100;
Cube myCubes[numCubes];

GLShader vertexShader, fragmentShader;

glm::mat4 viewMatrix, projectionMatrix;

GLuint lightPositionUniform, lightColorUniform;

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
    Cube::mvpUniformLocation = glGetUniformLocation(shaderIds[0], "mvp");
    Cube::colorUniformLocation = glGetUniformLocation(shaderIds[0], "color");
    Cube::modelUniformLocation = glGetUniformLocation(shaderIds[0], "modelMatrix");

    lightPositionUniform = glGetUniformLocation(shaderIds[0], "light.position");
    lightColorUniform = glGetUniformLocation(shaderIds[0], "light.intensities");

    exitOnGLError("ERROR: Could not get the shader uniform locations");

    glGenVertexArrays(1, &bufferIds[0]);
    glBindVertexArray(bufferIds[0]);
}

/*void initTexture() {
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures.width, textures.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures.pixel_data);

    textureUniform = glGetUniformLocation(shaderIds[0], "texture");
    glUniform1i(textureUniform, 0);
}*/

void freeResources(void) {
    glDetachShader(shaderIds[0], vertexShader.getShaderID());
    glDetachShader(shaderIds[0], fragmentShader.getShaderID());
    glDeleteShader(vertexShader.getShaderID());
    glDeleteShader(fragmentShader.getShaderID());
    glDeleteProgram(shaderIds[0]);
    exitOnGLError("ERROR: Could not destroy the shaders");

    Cube::freeGLResources();
    exitOnGLError("ERROR: Could not destroy the buffer objects");
}

void clearGraphics(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render(float timeLapsed) {
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

    int sqrtCubes = sqrt(numCubes);
    for (int i = 0; i < sqrtCubes; i++) {
        for (int j = 0; j < sqrtCubes; j++) {
            myCubes[i*sqrtCubes + j].translateCube(glm::vec3(i*2, 0, j*2));
        }
    }

    myLight.position = glm::vec3(0, 2, 0);
    myLight.intensities = glm::vec3(1, 1, 1);
    lightCube.translateCube(glm::vec3(0,2,0));
    lightCube.scaleCube(glm::vec3(.1, .1, .1));

    bigCube.scaleCube(glm::vec3(.3, 15, 30));
    bigCube.translateCube(glm::vec3(-5, 0, 0));
    bigCube.color = glm::vec3(0, 1, 0);

    floorCube.scaleCube(glm::vec3(40, 1, 40));
    floorCube.translateCube(glm::vec3(0, -3, 0));
    floorCube.color = glm::vec3(0, 0, 1);

    glUniform3fv(lightPositionUniform, 1, glm::value_ptr(myLight.position));
    glUniform3fv(lightColorUniform, 1, glm::value_ptr(myLight.intensities));

    printf("starting run loop\n");
    while (!userQuit()) {

        exitOnGLError("error before time check");
        Uint32 now = SDL_GetTicks();

        if (lastTime == 0) lastTime = now;
        Uint32 msLapsed = now - lastTime;
        lastTime = now;

        for (int i = 0; i < numCubes; i++) {
            myCubes[i].rotateCube(glm::vec3(cubeRotation*msLapsed));
        }

        exitOnGLError("error before player input proccessed");
        checkForPlayerInput();
        viewMatrix = updatePlayerView(msLapsed);

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
