#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;
out vec2 CloudCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = aPos;
    CloudCoords = vec2(aPos);
    gl_Position = projection * view * vec4(aPos, 1.0);
}