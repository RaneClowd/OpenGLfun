#ifndef OBJECT_H
#define OBJECT_H

#include "../Utils.h"

typedef struct {
    GLuint index;
    GLuint length;
} Range;

class Object
{
    public:
        Object();
        virtual ~Object(); // virtual means that child classes should be checked for the method first when calling it on the base class

        void drawToGL(void);
        void setColor(glm::vec4 color);

    protected: // can be accessed by derived classes
        virtual void defineShapeData(void) = 0;

        Range addVertexData(Vertex*, int size);
        Range addIndexData(GLuint*, int size);

    private: // can only be accessed by members of the same class or by 'friends'
        void initGLShape(void);

        GLuint vertexBufferID;
        GLuint indexBufferID;
        GLuint colorBufferID;

        int numIndices;
        Vertex *vertexData;
        GLuint *indexData;
};

#endif // OBJECT_H
