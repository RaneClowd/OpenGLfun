#include "GLShader.h"
#include <stdio.h>
#include <vector>
#include "../Utils.h"

GLShader::GLShader() {
    // default constructor?
}

bool GLShader::loadShader(std::string filePath, int shaderType) {
    FILE *file = fopen(filePath.c_str(), "rb"); // TODO: find diff between 'rt' and 'rb'

    if (!file) {
        fprintf(stderr, "ERROR: Could not get file handle.\n");
        perror("Error:");
        return false;
    }

    std::vector<std::string> shaderLines;
    char shaderLine[255];
    while(fgets(shaderLine, 255, file)) shaderLines.push_back(shaderLine);
    fclose(file);

    const char** shaderCode = new const char*[shaderLines.size()];
    for (uint i = 0; i < shaderLines.size(); i++) {
        shaderCode[i] = shaderLines[i].c_str();
    }

    if (0 == (__shaderId = glCreateShader(shaderType))) {
        fprintf(stderr, "ERROR: OpenGL Could not create shader slot");
    }

    glShaderSource(__shaderId, shaderLines.size(), shaderCode, NULL);
    glCompileShader(__shaderId);
    exitOnGLError("Could not compile shader");

    delete[] shaderCode;

    int iCompilationStatus;
    glGetShaderiv(__shaderId, GL_COMPILE_STATUS, &iCompilationStatus);

    if (iCompilationStatus == GL_FALSE) return false;
    __shaderType = shaderType;
    __loaded = true;

    return true;
}

void GLShader::deleteShader() {
    glDeleteShader(__shaderId);
}

bool GLShader::isLoaded() {
    return __loaded;
}

GLuint GLShader::getShaderID() {
    return __shaderId;
}
