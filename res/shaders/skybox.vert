#version 330

layout(location = 0) in vec3 position; // Position of the vertex

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

out vec3 texc;

void main(){
    vec4 pos = p * v * m * vec4(position, 1.0);
    texc = position;
    gl_Position = pos.xyww;
}
