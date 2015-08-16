#include "DrawableObject.h"

glm::mat4 DrawableObject::viewProjectionMatrix = IDENTITY_MATRIX;

DrawableObject::DrawableObject()
{
    //ctor
}

DrawableObject::~DrawableObject()
{
    //dtor
}

void DrawableObject::translate(glm::vec3 vec) {
    Object::translate(vec);
    this->recreateModelMatrix();
}

void DrawableObject::rotate(glm::vec3 vec) {
    Object::rotate(vec);
    this->recreateModelMatrix();
}

void DrawableObject::scale(glm::vec3 vec) {
    this->scaleVec.x *= vec.x;
    this->scaleVec.y *= vec.y;
    this->scaleVec.z *= vec.z;

    this->recreateModelMatrix();
}

void DrawableObject::recreateModelMatrix(void) {
    this->modelMatrix = glm::mat4(
                              1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              0, 0, 0, 1);

    this->modelMatrix = glm::translate(this->modelMatrix, this->translationVec);

    this->modelMatrix = glm::rotate(this->modelMatrix, this->rotationVec.x, glm::vec3(1, 0, 0));
    this->modelMatrix = glm::rotate(this->modelMatrix, this->rotationVec.y, glm::vec3(0, 1, 0));
    this->modelMatrix = glm::rotate(this->modelMatrix, this->rotationVec.z, glm::vec3(0, 0, 1));

    this->modelMatrix = glm::scale(this->modelMatrix, this->scaleVec);
}
