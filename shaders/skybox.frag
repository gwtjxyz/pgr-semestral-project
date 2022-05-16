#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

uniform sampler2D clouds;

void main() {
    FragColor = texture(skybox, TexCoords);
}