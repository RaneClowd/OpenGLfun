#include "Object.h"

Object::Object() {
    defineShapeData();
}

Object::~Object() {
    // clean up gl stuff here
}

void Object::drawToGL(void) {
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, modelMatrix);
    // TODO: uniform in color value

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

    exitOnGLError("ERROR: Could not draw an object");
}

void Object::initGLBuffers(void) {
    defineShapeData();
    
    glGenBuffers(1, )
    glGenBuffers(1, &vertexBufferID);
    glGenBuffers(1, &indexBufferID);
    
    
}
