#version 400 core

uniform float firstPass;
uniform sampler2D prevPos;
uniform sampler2D prevVel;
uniform int numParticles;

// output from quad.vert
in vec2 uv;

// TODO [Task 15] setup the output locations
layout(location = 0) out vec4 pos;
layout(location = 1) out vec4 vel;
const float PI = 3.14159;
const float dt = 0.0167; // 1 sec/60 fps

/*
    A particle has the following properties:
        - pos.xyz = clip space position
        - pos.w = lifetime in seconds (doesn't change)
        - vel.xyz = clip space velocity
        - vel.w = current age in seconds
*/

// A helpful procedural "random" number generator
float hash(float n) { return fract(sin(n)*753.5453123); }

// Helper functions to procedurally generate lifetimes and initial velocities
// based on particle index
float calculateLifetime(int index) {
    const float MAX_LIFETIME = 5.0;
    const float MIN_LIFETIME = 0.5;
    return MIN_LIFETIME + (MAX_LIFETIME - MIN_LIFETIME) * hash(index * 2349.2693);
}

vec2 calculateInitialVelocity(int index) {
    float theta = PI * hash(index * 872.0238);
    const float MAX_VEL = 0.3;
    float velMag = MAX_VEL * hash(index * 98723.345);
    return velMag * vec2(cos(theta), sin(theta));
}

vec4 initPosition(int index) {
    return vec4(3.f * calculateInitialVelocity(index).x, 2.f, 0.f, calculateLifetime(index));
    //return vec4(calculateInitialVelocity(index), 0.f, calculateLifetime(index));
}

vec4 initVelocity(int index) {
    vec2 rand = calculateInitialVelocity(index);
    return vec4(rand.x, -rand.y, 0, 1);
}

vec4 updatePosition(int index) {
    // TODO [Task 16]
    // - sample prevPos and prevVel at uv
    // - xyz: pos + vel * dt
    // - w component is lifetime, so keep it from the previous position
    vec2 p = texture(prevPos, uv).xy;
    vec2 v = texture(prevVel, uv).xy;
    vec2 xy = p + v * dt;
    return vec4(xy, 0.f, texture(prevPos, uv).w);
}

vec4 updateVelocity(int index) {
    const float G = -0.1;
    // TODO [Task 16]
    // - sample prevVel at uv
    // - only force is gravity in y direction.  Add G * dt.
    // - w component is age, so add dt
    vec2 v = texture(prevVel, uv).xy;
    v.y = v.y + G * dt;
    return vec4(v, texture(prevVel, uv).z + (dt / 7.f), texture(prevVel, uv).w);
}

void main() {
    int index = int(uv.x * numParticles);
    if (firstPass > 0.5) {
        pos = initPosition(index);
        pos.y += .2f;
        vel = initVelocity(index);
    } else {
        pos = updatePosition(index);
        vel = updateVelocity(index);

        if (pos.w < vel.z) {
            pos = initPosition(index);
            pos.y += .2f;
            vel = initVelocity(index);
        }
    }
}
