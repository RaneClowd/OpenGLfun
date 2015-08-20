#ifndef CUBE_H
#define CUBE_H

#include "../Utils.h"
#include "../GLHelpers/GLProgram.h"

// static members are shared between parent and children classes

class Cube
{
    public:
        Cube();
        //~Cube(); // virtual means that child classes should be checked for the method first when calling it on the base class

        // DrawableObject.h
        static void freeShapeResources();
        static void defineData();
        static GLuint getVertexArrayObjectID();
        static GLuint getNumDataIndices();

    protected: // can be accessed by derived classes

    private: // can only be accessed by members of the same class or by 'friends'

};

#endif // CUBE_H
