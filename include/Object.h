#ifndef OBJECT_H
#define OBJECT_H

#include "DrawableObject.h"

template <class T>
class Object : public T
{
    public:
        Object();
        virtual ~Object();

        void translate(glm::vec3);
        void rotate(glm::vec3);

    protected:

    private:
};

template class Object< DrawableObject <Cube> >;

#endif // OBJECT_H
