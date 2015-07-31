#ifndef CUBE_H
#define CUBE_H

#include "Object.h"
#include "../Utils.h"

// static members are shared between parent and children classes

class Cube
{
    public:
        Cube();
        virtual ~Cube(); // virtual means that child classes should be checked for the method first when calling it on the base class
    
        glm::vec3 color;
    
        void drawToGL(void);
    
        static GLuint modelMatrixUniformLocation;
        static GLuint colorValueUniformLocation;
    
    protected: // can be accessed by derived classes
    
    private: // can only be accessed by members of the same class or by 'friends'
        static GLuint vertexArrayObjectID = 0;
        static GLuint vertexBufferID = 0;
        static GLuint indexBufferID = 0;
    
        static int numIndices;
};

#endif // CUBE_H
