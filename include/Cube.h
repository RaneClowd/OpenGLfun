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
        //~Cube(); // virtual means that child classes should be checked for the method first when calling it on the base class

        void drawToGL(void);

        static void initGLResources(void);
        static void freeGLResources(void);

        GLProgram* shaderProgram;

    protected: // can be accessed by derived classes

    private: // can only be accessed by members of the same class or by 'friends'
        static GLuint vertexArrayObjectID;
        static GLuint vertexBufferID;
        static GLuint indexBufferID;

        static int numIndices;

};

#endif // CUBE_H
