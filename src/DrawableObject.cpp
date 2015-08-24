#include "DrawableObject.h"
#include "Cube.h"

glm::mat4 DrawableObject::viewProjectionMatrix = IDENTITY_MATRIX;
GLProgram *DrawableObject::shaderProgram = 0;

DrawableObject::DrawableObject()
{
    this->scaleVec = glm::vec3(1);
    this->recreateModelMatrix();
}

DrawableObject::~DrawableObject() { }

DrawableObject* DrawableObject::cubeObject() {
    Cube *newCube = new Cube;
    newCube->incrementObjectCount();
    return newCube;
}

void DrawableObject::releaseDrawableObject(DrawableObject* drawable) {
    drawable->decrementObjectCount();
    delete drawable;
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

    this->modelMatrix = glm::translate(this->modelMatrix, this->_translationVec);

    this->modelMatrix = glm::rotate(this->modelMatrix, this->_rotationVec.x, glm::vec3(1, 0, 0));
    this->modelMatrix = glm::rotate(this->modelMatrix, this->_rotationVec.y, glm::vec3(0, 1, 0));
    this->modelMatrix = glm::rotate(this->modelMatrix, this->_rotationVec.z, glm::vec3(0, 0, 1));

    this->modelMatrix = glm::scale(this->modelMatrix, this->scaleVec);
}

void DrawableObject::drawToGL(void) {
    GLuint voaID = this->getVertexArrayObjectID();
    if (voaID == -1) {
        this->defineData();
        voaID = this->getVertexArrayObjectID();
    }
    glBindVertexArray(voaID);

    glm::mat4 mvpMatrix = DrawableObject::viewProjectionMatrix * this->modelMatrix;
    DrawableObject::shaderProgram->loadToUniform("mvp", mvpMatrix);
    DrawableObject::shaderProgram->loadToUniform("modelMatrix", this->modelMatrix);

    DrawableObject::shaderProgram->loadToUniform("color", this->color);

    DrawableObject::shaderProgram->loadToUniformb("useTexture", false);

    glBindVertexArray(voaID);
    glDrawElements(GL_TRIANGLES, this->getNumDataIndices(), GL_UNSIGNED_INT, NULL);

    exitOnGLError("ERROR: Could not draw an object");
}
