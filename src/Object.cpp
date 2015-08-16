#include "Object.h"

Object::Object()
{
    this->translationVec = glm::vec3(0);
    this->rotationVec = glm::vec3(0);
}

Object::~Object() { }


void Object::translate(glm::vec3 vec) {
    this->translationVec.x += vec.x;
    this->translationVec.y += vec.y;
    this->translationVec.z += vec.z;
}

void Object::rotate(glm::vec3 vec) {
    this->rotationVec.x += vec.x;
    this->rotationVec.y += vec.y;
    this->rotationVec.z += vec.z;
}
