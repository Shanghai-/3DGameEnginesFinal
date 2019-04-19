#version 330
out vec4 fragColor;

in vec3 texc;

uniform samplerCube skyboxTexture;

void main(){
    fragColor = texture(skyboxTexture, texc);
    //fragColor = vec4(1.0);
}
