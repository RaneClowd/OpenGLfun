#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "../Utils.h"
#include "GLShader.h"

class GLProgram
{
    public:
        GLProgram();
        virtual ~GLProgram();

        bool loadShader(std::string shaderFile, GLuint shaderType);
        void linkAndUse(void);

        void loadToUniform(const GLchar* uniformName, glm::vec3 uniformValue);
        void loadToUniform(const GLchar* uniformName, glm::mat4 uniformValue);
        void loadToUniformf(const GLchar* uniformName, float uniformValue);
        void loadToUniformb(const GLchar* uniformName, bool uniformValue);

    protected:
    private:
        GLShader shaders[2];
        int shaderCount;

        GLuint programID;

        bool initializeGLProgram(void);
};

#endif // GLPROGRAM_H
