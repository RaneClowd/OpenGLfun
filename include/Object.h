#ifndef OBJECT_H
#define OBJECT_H

#include "../Utils.h"

/*

Every new cube in the world will need vertex/index data, color, model/transform matrices


Every instance of a cube should load its own color and model transform before drawing

The cube class should be responsible for the shared vertex/index data


Static members on the base class are used on derived classes, they aren't inherited :(

*/


typedef struct {
    GLuint index;
    GLuint length;
} Range;

class Object
{
    public:
        Object();
        virtual ~Object(); // virtual means that child classes should be checked for the method first when calling it on the base class

        glm::vec3 color;

        void drawToGL(void);

    protected: // can be accessed by derived classes
        virtual void defineShapeData(void) = 0;

        static Range addVertexData(Vertex*, int size);
        static Range addIndexData(GLuint*, int size);

    private: // can only be accessed by members of the same class or by 'friends'
        void initGLBuffers(void);

        GLuint vertexBufferID;
        GLuint indexBufferID;
        GLuint colorBufferID;

        static int numIndices;
        static Vertex *vertexData;
        static GLuint *indexData;
};

#endif // OBJECT_H
