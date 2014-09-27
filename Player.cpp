#include "Player.h"


unsigned char walkDirection = NONE;
float locationX=0, locationY=0, locationZ=-2;
#define MOVE_SPEED .1f;

float horizontalAngle = 0.0f, verticalAngle = 0.0f;
#define LOOK_SPEED .01f;

float fallAccel = -9.8, upVelocity = 0.0f;
clock_t lastClock = 0;
char jumpDetected = 0;

char userQuitFlag = 0;

void initPlayerInput(void) {
    if (SDL_SetRelativeMouseMode(SDL_TRUE)) {
        fprintf(stderr, "SDL unable to set relative mouse mode. ERROR: %s", SDL_GetError());
    }
}

Matrix updatePlayerView(void) {
    if (walkDirection & FORWARD) {
        locationZ += cos(horizontalAngle) * MOVE_SPEED;
        locationX -= sin(horizontalAngle) * MOVE_SPEED;
    }
    if (walkDirection & BACKWARD) {
        locationZ -= cos(horizontalAngle) * MOVE_SPEED;
        locationX += sin(horizontalAngle) * MOVE_SPEED;
    }

    if (walkDirection & LEFT) {
        locationX += cos(horizontalAngle) * MOVE_SPEED;
        locationZ += sin(horizontalAngle) * MOVE_SPEED;
    }
    if (walkDirection & RIGHT) {
        locationX -= cos(horizontalAngle) * MOVE_SPEED;
        locationZ -= sin(horizontalAngle) * MOVE_SPEED;
    }

    clock_t nowClock = clock();
    if (lastClock != 0 && (upVelocity != 0 || locationY != 0)) {
        float seconds = ((float)(nowClock - lastClock) / CLOCKS_PER_SEC);
        locationY -= upVelocity;

        printf("%f\t+\t%f\t=\n", upVelocity, fallAccel * seconds);
        upVelocity += fallAccel * seconds;

        if (locationY > 0) {
            locationY = 0;
            upVelocity = 0;
        }
    }
    lastClock = nowClock;

    Matrix viewMatrix = IDENTITY_MATRIX;
    translateMatrix(&viewMatrix, locationX, locationY, locationZ);
    rotateAboutY(&viewMatrix, horizontalAngle);
    rotateAboutX(&viewMatrix, verticalAngle);

    return viewMatrix;
}

void checkForPlayerInput(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) {
            userQuitFlag = 1;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W: {
                    walkDirection |= FORWARD;
                    break;
                }
                case SDL_SCANCODE_S: {
                    walkDirection |= BACKWARD;
                    break;
                }
                case SDL_SCANCODE_D: {
                    walkDirection |= RIGHT;
                    break;
                }
                case SDL_SCANCODE_A: {
                    walkDirection |= LEFT;
                    break;
                }
                case SDL_SCANCODE_SPACE: {
                    if (!jumpDetected && locationY == 0) {
                        upVelocity = .1;
                        jumpDetected = 1;
                    }
                    break;
                }
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W: {
                    walkDirection &= ~FORWARD;
                    break;
                }
                case SDL_SCANCODE_S: {
                    walkDirection &= ~BACKWARD;
                    break;
                }
                case SDL_SCANCODE_D: {
                    walkDirection &= ~RIGHT;
                    break;
                }
                case SDL_SCANCODE_A: {
                    walkDirection &= ~LEFT;
                    break;
                }
                case SDL_SCANCODE_SPACE: {
                    if (locationY == 0) jumpDetected = 0;
                    break;
                }
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            horizontalAngle += event.motion.xrel * LOOK_SPEED;
            verticalAngle -= event.motion.yrel * LOOK_SPEED;
        }
    }
}

char userQuit() {
    return userQuitFlag;
}
