#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 1) in vec3 normal;   // Normal of the vertex

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform float time;

out vec4 normal_worldSpace;
out vec4 position_worldSpace;
out vec4 basePosition_worldSpace;
out vec4 eye_worldSpace;

float surfaceCalc(float xPos) {
    float yDisp = (sin(xPos * 1.0 + time * 1.0)
                 + sin(xPos * 2.3 + time * 1.5)
                 + sin(xPos * 3.3 + time * 0.4)) / 3.0;
    return yDisp * 0.7;
}

void main(){
    //float yChange = 0.9 * sin(time * 0.5 + position.z + position.x) + 0.2 * sin(time + position.z + position.x);
    //yChange = yChange * 0.5;
    //float zChange = sin(time * 0.7 + position.z + position.x) * 0.3;
    //vec4 newPos = vec4(position.x - zChange, position.y + yChange, position.z + zChange, 1.0);
    vec4 newPos = vec4(position.x, surfaceCalc(position.x), position.z, 1.0);
    float baseY = surfaceCalc(0.0);
    newPos.y -= baseY;

    position_worldSpace = m * newPos;
    basePosition_worldSpace = m * vec4(position.x, baseY, position.z, 1.0);
    normal_worldSpace = vec4(normalize(mat3(transpose(inverse(m))) * normal), 0);
    eye_worldSpace = inverse(v)*vec4(0,0,0,1);
    gl_Position = p * v * m * newPos;
}
