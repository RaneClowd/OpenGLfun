#include "Cube.h"

void Cube::Cube() {
    // if shape data not defined, then define it
    if (vertexArrayObjectID == 0) {
        glm::vec4 VERTICES[36];
        
        // Lower front left
        VERTICES[0] = { -.5f, -.5f, -.5f, 1 };
        VERTICES[1] = { -.5f, -.5f, -.5f, 1 };
        VERTICES[2] = { -.5f, -.5f, -.5f, 1 };
        
        // Lower front right
        VERTICES[3] = {  .5f, -.5f, -.5f, 1 };
        VERTICES[4] = {  .5f, -.5f, -.5f, 1 };
        VERTICES[5] = {  .5f, -.5f, -.5f, 1 };
        
        // Upper front left
        VERTICES[6] = { -.5f,  .5f, -.5f, 1 };
        VERTICES[7] = { -.5f,  .5f, -.5f, 1 };
        VERTICES[8] = { -.5f,  .5f, -.5f, 1 };

        // Upper front right
        VERTICES[9] = {  .5f,  .5f, -.5f, 1 };
        VERTICES[10] = {  .5f,  .5f, -.5f, 1 };
        VERTICES[11] = {  .5f,  .5f, -.5f, 1 };

        // Lower back left
        VERTICES[12] = { -.5f, -.5f,  .5f, 1 };
        VERTICES[13] = { -.5f, -.5f,  .5f, 1 };
        VERTICES[14] = { -.5f, -.5f,  .5f, 1 };

        // Lower back right
        VERTICES[15] = {  .5f, -.5f,  .5f, 1 };
        VERTICES[16] = {  .5f, -.5f,  .5f, 1 };
        VERTICES[17] = {  .5f, -.5f,  .5f, 1 };

        // Upper back left
        VERTICES[18] = { -.5f,  .5f,  .5f, 1 };
        VERTICES[19] = { -.5f,  .5f,  .5f, 1 };
        VERTICES[20] = { -.5f,  .5f,  .5f, 1 };
        
        // Upper back right
        VERTICES[21] = {  .5f,  .5f,  .5f, 1 };
        VERTICES[22] = {  .5f,  .5f,  .5f, 1 };
        VERTICES[23] = {  .5f,  .5f,  .5f, 1 };
        
        const GLuint INDICES[72] = {
            4, 1, 9,  1, 6, 9,
            0, 3,17, 17,14, 0,
            2,13,20, 20, 7, 2,
            8,22,10,  8,19,22,
            16, 5,11, 16,11,23,
            15,18,12, 15,21,18,
        };
        
        glGenBuffers(1, &vertexBufferID);
        glGenBuffers(1, &indexBufferID);
        
        glGenVertexArrays(1, &vertexArrayObjectID);
        glBindVertexArray(vertexArrayObjectID);
        
        
        // Set up vertex data
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), NULL);
        
        
        // Set up index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
    }
}

Cube::~Cube() {
    // clean up gl stuff here
}

void Cube::drawToGL(void) {
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, modelMatrix);
    // TODO: uniform in color value
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
    
    exitOnGLError("ERROR: Could not draw an object");
}