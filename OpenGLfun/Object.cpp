//
//  Objects.cpp
//  OpenGLfun
//
//  Created by Kenny Skaggs on 9/30/14.
//
//

#include "Object.h"

Object::Object(const char* name) {
    _name = name;
    initShapeData();
}

void Object::drawToGL(void) {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    exitOnGLError("ERROR: Could not draw an object");
}

void Object::initShapeData() {
    
}