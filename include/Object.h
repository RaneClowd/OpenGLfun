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

        glm::vec3 getLocation();
        glm::vec3 getRotation();


    protected:
        glm::vec3 _translationVec;
        glm::vec3 _rotationVec;

    private:
};

#endif // OBJECT_H
