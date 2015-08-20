#include "Object.h"

template <class T>
Object<T>::Object()
{
    this->translationVec = glm::vec3(0);
    this->rotationVec = glm::vec3(0);
}

template <class T>
Object<T>::~Object() { }

template <class T>
void Object<T>::translate(glm::vec3 vec) {
    T::translationVec.x += vec.x;
    T::translationVec.y += vec.y;
    T::translationVec.z += vec.z;
    T::translate(vec);
}

template <class T>
void Object<T>::rotate(glm::vec3 vec) {
    T::rotationVec.x += vec.x;
    T::rotationVec.y += vec.y;
    T::rotationVec.z += vec.z;
    T::rotate(vec);
}
