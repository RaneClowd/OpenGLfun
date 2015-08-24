#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "../GLHelpers/GLProgram.h"
#include "Object.h"

class DrawableObject : public Object
{
    public:
        DrawableObject();
        virtual ~DrawableObject();

        static glm::mat4 viewProjectionMatrix;

        glm::vec3 color;

        // From Object.h (won't be needed when objects don't draw themselves)
        void translate(glm::vec3);
        void rotate(glm::vec3);

        void scale(glm::vec3);

        static GLProgram *shaderProgram;
        void drawToGL(void);

        // TODO: This should definitely be in a some sort of Scene object
        static DrawableObject* cubeObject();
        static void releaseDrawableObject(DrawableObject *drawable);


    protected:
        glm::vec3 scaleVec;

        // TODO: break drawing out into something else. It might be easier if objects didn't draw themselves
        void recreateModelMatrix();
        glm::mat4 modelMatrix;

        virtual GLuint getVertexArrayObjectID() = 0;
        virtual void defineData() = 0;
        virtual GLuint getNumDataIndices() = 0;

        virtual void freeShapeResources() = 0;

        virtual void incrementObjectCount() = 0;
        virtual void decrementObjectCount() = 0;

    private:
};

#endif // DRAWABLEOBJECT_H
