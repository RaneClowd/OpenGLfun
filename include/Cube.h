#ifndef CUBE_H
#define CUBE_H

#include "../Utils.h"
#include "../GLHelpers/GLProgram.h"
#include "DrawableObject.h"

// static members are shared between parent and children classes

class Cube : public DrawableObject
{
    public:
        Cube();
        ~Cube();


    protected:
        GLuint getVertexArrayObjectID();
        void defineData();
        GLuint getNumDataIndices();

        void freeShapeResources();

        uint getObjectCount();
        void incrementObjectCount();
        void decrementObjectCount();

    friend DrawableObject;

};

#endif // CUBE_H
