#include "Object.h"

Object::Object() {
    defineShapeData();
}

Object::~Object() {
    // clean up gl stuff here
}

void Object::drawToGL(void) {
    // TODO: load color to uniform

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

    exitOnGLError("ERROR: Could not draw an object");
}

Range Object::addVertexData(Vertex* vertexData, int size) {
    // add vertices to static vertexData mutable array
}

Range Object::addIndexData(GLuint* indexData, int size) {
    // add indices to static indexData mutable array
    numIndices += size;
}

void Object::initGLBuffers(void) {
    // init buffer objects with OpenGL
}
