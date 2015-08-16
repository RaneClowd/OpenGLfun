#ifndef OBJECT_H
#define OBJECT_H

#include "../Utils.h"

class Object
{
    public:
        Object();
        virtual ~Object();

        virtual void translate(glm::vec3);
        virtual void rotate(glm::vec3);

    protected:
        glm::vec3 translationVec;
        glm::vec3 rotationVec;

    private:
};

#endif // OBJECT_H
