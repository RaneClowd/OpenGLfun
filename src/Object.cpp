#include "Object.h"

Object::Object()
{
    this->_translationVec = glm::vec3(0);
    this->_rotationVec = glm::vec3(0);
}

Object::~Object() { }


glm::vec3 Object::getLocation() {
    return _translationVec;
}

glm::vec3 Object::getRotation() {
    return _rotationVec;
}

void Object::translate(glm::vec3 vec) {
    _translationVec.x += vec.x;
    _translationVec.y += vec.y;
    _translationVec.z += vec.z;
}

void Object::rotate(glm::vec3 vec) {
    _rotationVec.x += vec.x;
    _rotationVec.y += vec.y;
    _rotationVec.z += vec.z;
}
