#ifndef CUBE_H
#define CUBE_H

#include "../Utils.h"

// static members are shared between parent and children classes

class Cube
{
    public:
        Cube();
        //~Cube(); // virtual means that child classes should be checked for the method first when calling it on the base class

        glm::vec3 color;

        void drawToGL(void);

        static glm::mat4 viewProjectionMatrix;

        static GLuint mvpUniformLocation;
        static GLuint colorUniformLocation;

        static void initGLResources(void);
        static void freeGLResources(void);

    protected: // can be accessed by derived classes

    private: // can only be accessed by members of the same class or by 'friends'
        static GLuint vertexArrayObjectID;
        static GLuint vertexBufferID;
        static GLuint indexBufferID;

        static int numIndices;

        glm::mat4 modelMatrix;
};

#endif // CUBE_H
