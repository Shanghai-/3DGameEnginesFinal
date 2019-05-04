#version 330 core
out vec4 fragColor;

// Additional information for lighting
in vec4 normal_worldSpace;
in vec4 position_worldSpace;
in vec4 basePosition_worldSpace;
in vec4 eye_worldSpace;

void main(){
    //eyeToPoint = position_worldSpace - eye_worldSpace;
    //normFromUp = dot(normal_worldSpace, vec3(0, 1, 0));
    //brightness = dot(vec3(eye_))

    /* float val = position_worldSpace.y - basePosition_worldSpace.y;
    float diff = clamp(val * val, 0, 1);
    fragColor = mix(vec4(0.117647, 0.4, 0.8588, 0.95),
                    vec4(0.23137, 0.8196, 0.898, 0.7), diff); */

    //vec3 color = mix(vec3(0.117647, 0.4, 0.8588), vec3(0.57647, 0.7, 0.8588),
        //dot(normal_worldSpace, vec4(0,1,0,0)));
    vec3 color = vec3(0.117647, 0.4, 0.8588);

    color = color * (1 - dot(normal_worldSpace,
        normalize(position_worldSpace - eye_worldSpace)));

    fragColor = vec4(color, 0.93);
}
