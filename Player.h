#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Utils.h"

void initPlayerInput(void);

Matrix updatedPlayerView(long microsPassed);

void checkForPlayerInput(void);
char userQuit(void);

#endif // PLAYER_H_INCLUDED
