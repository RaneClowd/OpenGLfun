#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Utils.h"

void initPlayerInput(void);

Matrix updatePlayerView(void);
void setPlayerMovement(MoveDirection movement);
void checkForPlayerInput(void);
char userQuit(void);

#endif // PLAYER_H_INCLUDED
