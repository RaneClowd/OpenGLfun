#include "Object.h"

Object::Object() {
    initShapeData();
}

Object::~Object() {
    // clean up gl stuff here
}

void Object::drawToGL(void) {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    exitOnGLError("ERROR: Could not draw an object");
}

void

void Object::initShapeData() {

}
