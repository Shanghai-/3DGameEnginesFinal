#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 1) in vec2 texCoord; // UV texture coordinates
layout(location = 2) in vec3 normal;   // Normal of the vertex
layout(location = 3) in ivec4 boneIDs; // IDs of all bones influencing this vert
layout(location = 4) in vec4 weights; // Weights of those influences

const int MAX_BONES = 100;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform mat4 boneTrans[MAX_BONES];

out vec4 normal_worldSpace;
out vec4 position_worldSpace;
out vec4 eye_worldSpace;
out vec2 texc;

void main(){
    mat4 boneTransform = boneTrans[boneIDs[0]] * weights[0];
    boneTransform += boneTrans[boneIDs[1]] * weights[1];
    boneTransform += boneTrans[boneIDs[2]] * weights[2];
    boneTransform += boneTrans[boneIDs[3]] * weights[3];

    /* mat4 boneTransform = boneTrans[boneIDs[0]] * 0.25;
    boneTransform += boneTrans[boneIDs[1]] * 0.25;
    boneTransform += boneTrans[boneIDs[2]] * 0.25;
    boneTransform += boneTrans[boneIDs[3]] * 0.25; */


    texc = texCoord;

    vec4 position_boned = boneTransform * vec4(position, 1.0);
    position_worldSpace = m * position_boned;
    //position_worldSpace = vec4(0.0, 0.0, 0.0, 0.0);

    normal_worldSpace = boneTransform * vec4(normalize(mat3(transpose(inverse(m))) * normal), 0);
    eye_worldSpace = inverse(v)*vec4(0,0,0,1);
    gl_Position = p * v * m * position_boned;
    //gl_Position = vec4(0.0, 0.0, 0.0, 0.0);//vec4(position, 1.0);
}
