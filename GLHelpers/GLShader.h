#ifndef GLSHADER_H
#define GLSHADER_H

#include <GL/glew.h>
#include <string>

class GLShader
{
    public:
        bool loadShader(std::string filePath, int shaderType);
        void deleteShader();

        bool isLoaded();
        GLuint getShaderID();

        GLShader();

    private:
        GLuint __shaderId;
        int __shaderType;
        bool __loaded;
        void printCompileError();
};

#endif // GLSHADER_H
