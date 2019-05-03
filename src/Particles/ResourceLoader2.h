#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"

class ResourceLoader2 {
public:
    ResourceLoader2();
    static GLuint createShaderProgram(const char * vertex_file_path,const char * fragment_file_path);
    static void initializeGlew();

private:
    static GLuint createShader(GLenum shaderType, const char *filepath);
};

#endif // SHADER_H
