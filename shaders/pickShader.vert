#version 330 core

layout (location = 0) in vec3 position;

out float depth;

uniform mat4 model;
uniform mat4 proj;

void main() {
    vec4 pos4 = model * vec4(position, 1.0f);
    gl_Position = proj * pos4;
    depth = pos4.z / pos4.w;
}