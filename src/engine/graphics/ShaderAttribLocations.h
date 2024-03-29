#ifndef SHADERATTRIBLOCATIONS_H
#define SHADERATTRIBLOCATIONS_H

#include "GL/glew.h"

/**
 *
 * ***IMPORTANT FOR WRITING SHADERS***
 *
 *
 * All shaders (CS1972's default and any of your custom ones) must specify the layout locations
 * to match the layout locations in that file.
 */
namespace ShaderAttrib {

    // Standard attributes
    const GLuint POSITION = 0;
    const GLuint NORMAL = 1;
    const GLuint TANGENT = 2;
    const GLuint COLOR = 3;
    const GLuint BINORMAL = 4;
    const GLuint TEXCOORD0 = 5;
    const GLuint TEXCOORD1 = 6;
    const GLuint TEXCOORD2 = 7;
    const GLuint TEXCOORD3 = 8;

    // Starting at this index,
    const GLuint SPECIAL0 = 9;
    const GLuint BONES = 10;
    const GLuint WEIGHTS = 11;
}

#endif // SHADERATTRIBLOCATIONS_H
