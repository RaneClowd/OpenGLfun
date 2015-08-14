#include "GLProgram.h"

GLProgram::GLProgram()
{
    this->shaderCount = 0;
    this->programID = -1;
}

GLProgram::~GLProgram()
{
    glDetachShader(this->programID, this->shaders[0].getShaderID());
    glDetachShader(this->programID, this->shaders[1].getShaderID());
    glDeleteShader(this->shaders[0].getShaderID());
    glDeleteShader(this->shaders[1].getShaderID());
    glDeleteProgram(this->programID);
    exitOnGLError("ERROR: Could not destroy the shaders problems!!");
    printf("shaders destroyed\n");
}

void GLProgram::loadToUniform(const GLchar* uniformName, glm::vec3 uniformValue) {
    GLuint uniformLocation = glGetUniformLocation(this->programID, uniformName);
    glUniform3fv(uniformLocation, 1, glm::value_ptr(uniformValue));
}

void GLProgram::loadToUniform(const GLchar* uniformName, glm::mat4 uniformValue) {
    GLuint uniformLocation = glGetUniformLocation(this->programID, uniformName);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(uniformValue));
}

void GLProgram::loadToUniform(const GLchar* uniformName, float uniformValue) {
    GLuint uniformLocation = glGetUniformLocation(this->programID, uniformName);
    glUniform1f(uniformLocation, uniformValue);
}

bool GLProgram::loadShader(std::string shaderFile, GLuint shaderType) {
    if (this->programID == -1) {
        this->initializeGLProgram();
        printf("initializing GL program\n");
    }

    GLShader *shaderSlot = &(this->shaders[this->shaderCount]);

    shaderSlot->loadShader(shaderFile, shaderType);
    glAttachShader(this->programID, shaderSlot->getShaderID());

    this->shaderCount++;
}

void GLProgram::linkAndUse(void) {
    glLinkProgram(this->programID);
    exitOnGLError("ERROR: Could not link the shader program");

    glUseProgram(this->programID);
    exitOnGLError("ERROR: Problem using compiled program");
}

bool GLProgram::initializeGLProgram(void) {
    this->programID = glCreateProgram();
}
