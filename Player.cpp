#include "Player.h"

#define INITIAL_HEIGHT 2

unsigned char walkDirection = NONE;
float xWorldShift=0, yWorldShift=-INITIAL_HEIGHT, zWorldShift=-4; // "It came to me in a dream. The engines don't move the ship at all. The ship stays where it is, and the engines move the universe around it." -Professor Cubert J. Farnsworth
#define MOVE_SPEED_PER_MICRO .001f

float horizontalAngle = 0.0f, verticalAngle = 0.0f;
#define LOOK_SPEED_PER_MICRO .001f

const float fallAccelPerMicro = -9.8 / 1000000;
float jumpVelocityPerMicro = 0.0f, xMouseMotion = 0.0f, yMouseMotion = 0.0f;

char userQuitFlag = 0;

void initPlayerInput(void) {
    if (SDL_SetRelativeMouseMode(SDL_TRUE)) {
        fprintf(stderr, "SDL unable to set relative mouse mode. ERROR: %s", SDL_GetError());
    }
}

Matrix updatedPlayerView(long microsPassed) {
    if (walkDirection & FORWARD) {
        zWorldShift += cos(horizontalAngle) * MOVE_SPEED_PER_MICRO * microsPassed;
        xWorldShift -= sin(horizontalAngle) * MOVE_SPEED_PER_MICRO * microsPassed;
    }
    if (walkDirection & BACKWARD) {
        zWorldShift -= cos(horizontalAngle) * MOVE_SPEED_PER_MICRO * microsPassed;
        xWorldShift += sin(horizontalAngle) * MOVE_SPEED_PER_MICRO * microsPassed;
    }

    if (walkDirection & LEFT) {
        xWorldShift += cos(horizontalAngle) * MOVE_SPEED_PER_MICRO * microsPassed;
        zWorldShift += sin(horizontalAngle) * MOVE_SPEED_PER_MICRO * microsPassed;
    }
    if (walkDirection & RIGHT) {
        xWorldShift -= cos(horizontalAngle) * MOVE_SPEED_PER_MICRO * microsPassed;
        zWorldShift -= sin(horizontalAngle) * MOVE_SPEED_PER_MICRO * microsPassed;
    }

    if (jumpVelocityPerMicro != 0 || yWorldShift != 0) {
        yWorldShift -= jumpVelocityPerMicro * microsPassed;

        jumpVelocityPerMicro += fallAccelPerMicro * microsPassed;

        if (yWorldShift > -INITIAL_HEIGHT) {
            yWorldShift = -INITIAL_HEIGHT; // at rest, the world is pushed down by how tall you are. because zero is defined by where your eyes are
            jumpVelocityPerMicro = 0;
        }
    }

    horizontalAngle += xMouseMotion * LOOK_SPEED_PER_MICRO;
    verticalAngle -= yMouseMotion * LOOK_SPEED_PER_MICRO;
    // TODO: does it make sense to use the microsPassed when updating the view angle? it's not like i'm using some sort of look-momentum
    // mechanic. It might get weird with the SDL events, I might actually be missing some mouse updates

    Matrix viewMatrix = IDENTITY_MATRIX;
    translateMatrix(&viewMatrix, xWorldShift, yWorldShift, zWorldShift);
    rotateAboutY(&viewMatrix, horizontalAngle);
    rotateAboutX(&viewMatrix, verticalAngle);

    return viewMatrix;
}

void checkForPlayerInput(void) {
    SDL_Event event;
    char debugMouseEventProcessed = 0;

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
                        jumpVelocityPerMicro = .3;
                    }
                    break;
                }
                default: { break; }
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
                default: { break; }
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            xMouseMotion = event.motion.xrel;
            yMouseMotion = event.motion.yrel;

            // Want to make sure I'm not stomping on mouse motion
            if (debugMouseEventProcessed) fprintf(stderr, "Mouse motion has been lost!");
            debugMouseEventProcessed = 1;
        }
    }
}

char userQuit() {
    return userQuitFlag;
}
