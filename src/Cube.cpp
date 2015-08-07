#include "Cube.h"

int Cube::numIndices = 36;

glm::mat4 Cube::viewProjectionMatrix = IDENTITY_MATRIX;

GLuint Cube::mvpUniformLocation = 0;
GLuint Cube::colorUniformLocation = 0;

GLuint Cube::vertexArrayObjectID = 0;
GLuint Cube::vertexBufferID = 0;
GLuint Cube::indexBufferID = 0;


Cube::Cube() {
    this->translationVec = glm::vec3(0, 0, 0);
    this->rotationVec = glm::vec3(0, 0, 0);
    this->recreateModelMatrix();

    this->color = {1, 0, 0};
}

void Cube::initGLResources(void) {
    Vertex VERTICES[36];

    // Lower front left
    VERTICES[0] = { { -.5f, -.5f, -.5f }, { 0, 0, 1 } };
    VERTICES[1] = { { -.5f, -.5f, -.5f }, { 1, 0, 0 } };
    VERTICES[2] = { { -.5f, -.5f, -.5f }, { 0, 1, 0 } };

    // Lower front right
    VERTICES[3] = { {  .5f, -.5f, -.5f }, { 0, 0, 1 } };
    VERTICES[4] = { {  .5f, -.5f, -.5f }, { 1, 0, 0 } };
    VERTICES[5] = { {  .5f, -.5f, -.5f }, { 1, 1, 0 } };

    // Upper front left
    VERTICES[6] = { { -.5f,  .5f, -.5f }, { 1, 0, 0 } };
    VERTICES[7] = { { -.5f,  .5f, -.5f }, { 0, 1, 0 } };
    VERTICES[8] = { { -.5f,  .5f, -.5f }, { 0, 0, 0 } };

    // Upper front right
    VERTICES[9] = { {  .5f,  .5f, -.5f }, { 1, 0, 0 } };
    VERTICES[10] = { {  .5f,  .5f, -.5f }, { 0, 0, 0 } };
    VERTICES[11] = { {  .5f,  .5f, -.5f }, { 1, 1, 0 } };

    // Lower back left
    VERTICES[12] = { { -.5f, -.5f,  .5f }, { 1, 0, 1 } };
    VERTICES[13] = { { -.5f, -.5f,  .5f }, { 0, 1, 0 } };
    VERTICES[14] = { { -.5f, -.5f,  .5f }, { 0, 0, 1 } };

    // Lower back right
    VERTICES[15] = { {  .5f, -.5f,  .5f }, { 1, 0, 1 } };
    VERTICES[16] = { {  .5f, -.5f,  .5f }, { 1, 1, 0 } };
    VERTICES[17] = { {  .5f, -.5f,  .5f }, { 0, 0, 1 } };

    // Upper back left
    VERTICES[18] = { { -.5f,  .5f,  .5f }, { 1, 0, 1 } };
    VERTICES[19] = { { -.5f,  .5f,  .5f }, { 0, 0, 0 } };
    VERTICES[20] = { { -.5f,  .5f,  .5f }, { 0, 1, 0 } };

    // Upper back right
    VERTICES[21] = { {  .5f,  .5f,  .5f }, { 1, 0, 1 } };
    VERTICES[22] = { {  .5f,  .5f,  .5f }, { 0, 0, 0 } };
    VERTICES[23] = { {  .5f,  .5f,  .5f }, { 1, 1, 0 } };

    const GLuint INDICES[Cube::numIndices] = {
        4, 1, 9,  1, 6, 9,
        0, 3,17, 17,14, 0,
        2,13,20, 20, 7, 2,
        8,22,10,  8,19,22,
       16, 5,11, 16,11,23,
       15,18,12, 15,21,18,
    };

    glGenBuffers(1, &Cube::vertexBufferID);
    glGenBuffers(1, &Cube::indexBufferID);

    glGenVertexArrays(1, &Cube::vertexArrayObjectID);
    glBindVertexArray(Cube::vertexArrayObjectID);


    // Set up vertex data
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, Cube::vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)sizeof(VERTICES[0].Position));


    // Set up index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Cube::indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Cube::freeGLResources(void) {
    glDeleteBuffers(1, &Cube::vertexBufferID);
    glDeleteBuffers(1, &Cube::indexBufferID);
    glDeleteVertexArrays(1, &Cube::vertexArrayObjectID);
}

void Cube::translateCube(glm::vec3 vec) {
    this->translationVec.x += vec.x;
    this->translationVec.y += vec.y;
    this->translationVec.z += vec.z;
    this->recreateModelMatrix();
}

void Cube::rotateCube(glm::vec3 vec) {
    this->rotationVec.x += vec.x;
    this->rotationVec.y += vec.y;
    this->rotationVec.z += vec.z;
    this->recreateModelMatrix();
}

void Cube::recreateModelMatrix(void) {
    this->modelMatrix = glm::mat4(
                              1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              0, 0, 0, 1);

    this->modelMatrix = glm::translate(this->modelMatrix, this->translationVec);

    this->modelMatrix = glm::rotate(this->modelMatrix, this->rotationVec.x, glm::vec3(1, 0, 0));
    this->modelMatrix = glm::rotate(this->modelMatrix, this->rotationVec.y, glm::vec3(0, 1, 0));
    this->modelMatrix = glm::rotate(this->modelMatrix, this->rotationVec.z, glm::vec3(0, 0, 1));
}

void Cube::drawToGL(void) {
    glm::mat4 mvpMatrix = Cube::viewProjectionMatrix * this->modelMatrix;
    glUniformMatrix4fv(Cube::mvpUniformLocation, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

    glUniform3fv(Cube::colorUniformLocation, 1, glm::value_ptr(this->color));

    glBindVertexArray(Cube::vertexArrayObjectID);
    glDrawElements(GL_TRIANGLES, Cube::numIndices, GL_UNSIGNED_INT, NULL);

    exitOnGLError("ERROR: Could not draw an object");
}
