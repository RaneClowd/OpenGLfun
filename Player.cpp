#include "Player.h"


const int INITIAL_HEIGHT = 2;
const float MOVE_SPEED = .01f, LOOK_SPEED = .01f, GRAVITY_FORCE = -.00005;

unsigned char walkDirection = NONE;
//glm::vec3 worldShift = glm::vec3(0, -INITIAL_HEIGHT, -4); // "It came to me in a dream. The engines don't move the ship at all. The ship stays where it is, and the engines move the universe around it." -Professor Cubert J. Farnsworth
glm::vec3 playerLocation = glm::vec3(0, INITIAL_HEIGHT, 4);

glm::vec3 angle = glm::vec3(0, -0.5, 0);

float jumpVelocity = 0.0f;
char jumpDetected = 0;

char userQuitFlag = 0;

void initPlayerInput(void) {
    if (SDL_SetRelativeMouseMode(SDL_TRUE)) {
        fprintf(stderr, "SDL unable to set relative mouse mode. ERROR: %s", SDL_GetError());
    }
}

glm::mat4 updatePlayerView(Uint32 msLapsed) {
    if (walkDirection & FORWARD) {
        playerLocation.z -= cosf(angle.x) * MOVE_SPEED * msLapsed;
        playerLocation.x += sinf(angle.x) * MOVE_SPEED * msLapsed;
    }
    if (walkDirection & BACKWARD) {
        playerLocation.z += cosf(angle.x) * MOVE_SPEED * msLapsed;
        playerLocation.x -= sinf(angle.x) * MOVE_SPEED * msLapsed;
    }

    if (walkDirection & LEFT) {
        playerLocation.x -= cosf(angle.x) * MOVE_SPEED * msLapsed;
        playerLocation.z -= sinf(angle.x) * MOVE_SPEED * msLapsed;
    }
    if (walkDirection & RIGHT) {
        playerLocation.x += cosf(angle.x) * MOVE_SPEED * msLapsed;
        playerLocation.z += sinf(angle.x) * MOVE_SPEED * msLapsed;
    }

    if (msLapsed != 0 && (jumpVelocity != 0 || playerLocation.y != INITIAL_HEIGHT)) {
        playerLocation.y += jumpVelocity * msLapsed;

        jumpVelocity += GRAVITY_FORCE * msLapsed;

        if (playerLocation.y < INITIAL_HEIGHT) {
            playerLocation.y = INITIAL_HEIGHT;
            jumpVelocity = 0;
        }
    }

    glm::mat4 view = glm::rotate(IDENTITY_MATRIX, angle.y, glm::vec3(-1, 0, 0));
    view = glm::rotate(view, angle.x, glm::vec3(0, 1, 0));

    view = glm::translate(view, -playerLocation); // "It came to me in a dream. The engines don't move the ship at all. The ship stays where it is, and the engines move the universe around it." -Professor Cubert J. Farnsworth

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
                    if (playerLocation.y == INITIAL_HEIGHT) {
                        jumpVelocity = .02;
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
