//
//  Objects.h
//  OpenGLfun
//
//  Created by Kenny Skaggs on 9/30/14.
//
//

#ifndef __OpenGLfun__Objects__
#define __OpenGLfun__Objects__
#include "Utils.h"

class Object {
    
private:
    const char* _name;
    uint numIndices;
    GLuint vertexBufferId, indexBufferId;
    
    void initShapeData(void);
    
    
public:
    Object(const char* name);
    void drawToGL();
    
};

#endif /* defined(__OpenGLfun__Objects__) */
