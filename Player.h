#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Utils.h"
#include "Object.h"

class Player : public Object
{
    public:
        Player();
        virtual ~Player();

        void initPlayerInput();
        glm::mat4 updatePlayerView(Uint32 msLapsed);

        void checkForPlayerInput();
        char userQuit();
};

#endif // PLAYER_H_INCLUDED
