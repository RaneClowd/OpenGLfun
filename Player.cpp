#include "Player.h"


MoveDirection movementState = NONE;
float locationX=0, locationY=0, locationZ=-2;
#define WALK_SPEED .1f;

float horizontalAngle = 0.0f, verticalAngle = 0.0f;
#define LOOK_SPEED .005f;

char userQuitFlag = 0;

void initPlayerInput(void) {
    if (SDL_SetRelativeMouseMode(SDL_TRUE)) {
        fprintf(stderr, "SDL unable to set relative mouse mode. ERROR: %s", SDL_GetError());
    }
}

Matrix updatePlayerView(void) {
    switch (movementState) {
        case FORWARD: {
            locationZ += cos(horizontalAngle) * WALK_SPEED;
            locationX -= sin(horizontalAngle) * WALK_SPEED;
            break;
        }
        case BACKWARD: {
            locationZ -= cos(horizontalAngle) * WALK_SPEED;
            locationX += sin(horizontalAngle) * WALK_SPEED;
            break;
        }
        case RIGHT: {
            locationX -= cos(horizontalAngle) * WALK_SPEED;
            locationZ -= sin(horizontalAngle) * WALK_SPEED;
            break;
        }
        case LEFT: {
            locationX += cos(horizontalAngle) * WALK_SPEED;
            locationZ += sin(horizontalAngle) * WALK_SPEED;
            break;
        }
    }

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
            switch (event.key.keysym.sym) {
                case SDLK_COMMA: {
                    movementState = FORWARD;
                    break;
                }
                case SDLK_o: {
                    movementState = BACKWARD;
                    break;
                }
                case SDLK_e: {
                    movementState = RIGHT;
                    break;
                }
                case SDLK_a: {
                    movementState = LEFT;
                    break;
                }
            }
        } else if (event.type == SDL_KEYUP) {
            movementState = NONE;
        } else if (event.type == SDL_MOUSEMOTION) {
            horizontalAngle += event.motion.xrel * LOOK_SPEED;
            verticalAngle -= event.motion.yrel * LOOK_SPEED;
        }
    }
}

char userQuit() {
    return userQuitFlag;
}
