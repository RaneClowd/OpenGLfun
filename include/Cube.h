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

        glm::vec3 color;

        static glm::mat4 viewProjectionMatrix;

        void translateCube(glm::vec3);
        void scaleCube(glm::vec3);
        void rotateCube(glm::vec3);

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

        glm::mat4 modelMatrix;

        glm::vec3 translationVec;
        glm::vec3 scaleVec;
        glm::vec3 rotationVec;
        void recreateModelMatrix(void);
};

#endif // CUBE_H
