#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include <Object.h>


class DrawableObject : public Object
{
    public:
        DrawableObject();
        virtual ~DrawableObject();

        static glm::mat4 viewProjectionMatrix;

        glm::vec3 color;

        // Object.h
        virtual void translate(glm::vec3);
        virtual void rotate(glm::vec3);

        virtual void scale(glm::vec3);

    protected:
        glm::vec3 scaleVec;

        virtual void recreateModelMatrix();
        glm::mat4 modelMatrix;

    private:
};

#endif // DRAWABLEOBJECT_H
