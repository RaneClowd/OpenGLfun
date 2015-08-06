#include "Player.h"


const int INITIAL_HEIGHT = 2;
const float MOVE_SPEED = .1f, LOOK_SPEED = .01f, GRAVITY_FORCE = -25.8;

unsigned char walkDirection = NONE;
glm::vec3 worldShift = glm::vec3(0, -INITIAL_HEIGHT, -4); // "It came to me in a dream. The engines don't move the ship at all. The ship stays where it is, and the engines move the universe around it." -Professor Cubert J. Farnsworth

glm::vec3 angle = glm::vec3(0, -0.5, 0);

float jumpVelocity = 0.0f;
char jumpDetected = 0;

char userQuitFlag = 0;

void initPlayerInput(void) {
    if (SDL_SetRelativeMouseMode(SDL_TRUE)) {
        fprintf(stderr, "SDL unable to set relative mouse mode. ERROR: %s", SDL_GetError());
    }
}

glm::mat4 updatePlayerView(float timeLapsed) {
    if (walkDirection & FORWARD) {
        worldShift.z += cosf(angle.x) * MOVE_SPEED;
        worldShift.x -= sinf(angle.x) * MOVE_SPEED;
    }
    if (walkDirection & BACKWARD) {
        worldShift.z -= cosf(angle.x) * MOVE_SPEED;
        worldShift.x += sinf(angle.x) * MOVE_SPEED;
    }

    if (walkDirection & LEFT) {
        worldShift.x += cosf(angle.x) * MOVE_SPEED;
        worldShift.z += sinf(angle.x) * MOVE_SPEED;
    }
    if (walkDirection & RIGHT) {
        worldShift.x -= cosf(angle.x) * MOVE_SPEED;
        worldShift.z -= sinf(angle.x) * MOVE_SPEED;
    }

    if (timeLapsed != 0 && (jumpVelocity != 0 || worldShift.y != 0)) {
        worldShift.y -= jumpVelocity;

        jumpVelocity += GRAVITY_FORCE * timeLapsed;

        if (worldShift.y > -INITIAL_HEIGHT) {
            worldShift.y = -INITIAL_HEIGHT; // at rest, the world is pushed down by how tall you are. because zero is defined by where your eyes are
            jumpVelocity = 0;
        }
    }

    glm::mat4 view = glm::rotate(IDENTITY_MATRIX, angle.y, glm::vec3(-1, 0, 0));
    view = glm::rotate(view, angle.x, glm::vec3(0, 1, 0));
    view = glm::translate(view, worldShift);

    return view;
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
                    if (worldShift.y == -INITIAL_HEIGHT) {
                        jumpVelocity = .3;
                        jumpDetected = 1;
                    }
                    break;
                }
                default: {}
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
                default: {}
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            angle.x += event.motion.xrel * LOOK_SPEED;
            angle.y -= event.motion.yrel * LOOK_SPEED;
        }
    }
}

char userQuit() {
    return userQuitFlag;
}
