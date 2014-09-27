#include "Player.h"

#define INITIAL_HEIGHT 2

unsigned char walkDirection = NONE;
float xWorldShift=0, yWorldShift=-INITIAL_HEIGHT, zWorldShift=-2; // "It came to me in a dream. The engines don't move the ship at all. The ship stays where it is, and the engines move the universe around it." -Professor Cubert J. Farnsworth
#define MOVE_SPEED .1f

float horizontalAngle = 0.0f, verticalAngle = 0.0f;
#define LOOK_SPEED .01f

float fallAccel = -25.8, jumpVelocity = 0.0f;
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
        zWorldShift += cos(horizontalAngle) * MOVE_SPEED;
        xWorldShift -= sin(horizontalAngle) * MOVE_SPEED;
    }
    if (walkDirection & BACKWARD) {
        zWorldShift -= cos(horizontalAngle) * MOVE_SPEED;
        xWorldShift += sin(horizontalAngle) * MOVE_SPEED;
    }

    if (walkDirection & LEFT) {
        xWorldShift += cos(horizontalAngle) * MOVE_SPEED;
        zWorldShift += sin(horizontalAngle) * MOVE_SPEED;
    }
    if (walkDirection & RIGHT) {
        xWorldShift -= cos(horizontalAngle) * MOVE_SPEED;
        zWorldShift -= sin(horizontalAngle) * MOVE_SPEED;
    }

    clock_t nowClock = clock();
    if (lastClock != 0 && (jumpVelocity != 0 || yWorldShift != 0)) {
        float seconds = ((float)(nowClock - lastClock) / CLOCKS_PER_SEC);
        yWorldShift -= jumpVelocity;

        jumpVelocity += fallAccel * seconds;

        if (yWorldShift > -INITIAL_HEIGHT) {
            yWorldShift = -INITIAL_HEIGHT; // at rest, the world is pushed down by how tall you are. because zero is defined by where your eyes are
            jumpVelocity = 0;
        }
    }
    lastClock = nowClock;

    Matrix viewMatrix = IDENTITY_MATRIX;
    translateMatrix(&viewMatrix, xWorldShift, yWorldShift, zWorldShift);
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
                    if (yWorldShift == -INITIAL_HEIGHT) {
                        jumpVelocity = .3;
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
                    jumpDetected = 0;
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
