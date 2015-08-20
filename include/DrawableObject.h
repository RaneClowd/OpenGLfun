#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "../GLHelpers/GLProgram.h"
#include "Cube.h"

template <class T>
class DrawableObject : public T
{
    public:
        DrawableObject();
        ~DrawableObject();

        static glm::mat4 viewProjectionMatrix;

        glm::vec3 color;

        void translate(glm::vec3);
        void rotate(glm::vec3);

        void scale(glm::vec3);

        static GLProgram *shaderProgram;
        void drawToGL(void);


    protected:
        glm::vec3 translationVec;
        glm::vec3 rotationVec;
        glm::vec3 scaleVec;

        void recreateModelMatrix();
        glm::mat4 modelMatrix;

    private:
};

template class DrawableObject<Cube>;

#endif // DRAWABLEOBJECT_H
