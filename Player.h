#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Utils.h"

void initPlayerInput(void);

glm::mat4 updatePlayerView(float timeLapsed);

void checkForPlayerInput(void);
char userQuit(void);

#endif // PLAYER_H_INCLUDED
