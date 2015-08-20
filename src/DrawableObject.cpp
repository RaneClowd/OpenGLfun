#include "DrawableObject.h"

template <class T>
glm::mat4 DrawableObject<T>::viewProjectionMatrix = IDENTITY_MATRIX;

template <class T>
GLProgram *DrawableObject<T>::shaderProgram = 0;

template <class T>
DrawableObject<T>::DrawableObject()
{
    this->scaleVec = glm::vec3(1);
    this->recreateModelMatrix();
}

template <class T>
DrawableObject<T>::~DrawableObject()
{
    //T::freeShapeResources();
}

template <class T>
void DrawableObject<T>::translate(glm::vec3 vec) {
    this->recreateModelMatrix();
}

template <class T>
void DrawableObject<T>::rotate(glm::vec3 vec) {
    this->recreateModelMatrix();
}

template <class T>
void DrawableObject<T>::scale(glm::vec3 vec) {
    this->scaleVec.x *= vec.x;
    this->scaleVec.y *= vec.y;
    this->scaleVec.z *= vec.z;

    this->recreateModelMatrix();
}

template <class T>
void DrawableObject<T>::recreateModelMatrix(void) {
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

template <class T>
void DrawableObject<T>::drawToGL(void) {
    GLuint voaID = T::getVertexArrayObjectID();
    if (voaID == -1) {
        T::defineData();
        voaID = T::getVertexArrayObjectID();
    }
    glBindVertexArray(voaID);

    glm::mat4 mvpMatrix = DrawableObject::viewProjectionMatrix * this->modelMatrix;
    DrawableObject::shaderProgram->loadToUniform("mvp", mvpMatrix);
    DrawableObject::shaderProgram->loadToUniform("modelMatrix", this->modelMatrix);

    DrawableObject::shaderProgram->loadToUniform("color", this->color);

    DrawableObject::shaderProgram->loadToUniformb("useTexture", false);

    glBindVertexArray(this->getVertexArrayObjectID());
    glDrawElements(GL_TRIANGLES, T::getNumDataIndices(), GL_UNSIGNED_INT, NULL);

    exitOnGLError("ERROR: Could not draw an object");
}
