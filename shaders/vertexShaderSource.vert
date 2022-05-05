#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 PVM;

void main() {
    gl_Position = PVM * vec4(aPos, 1.0f);
    FragPos = vec3(model * vec4(aPos, 1.0));
    // gets rid of offsets caused by non-uniform scaling
    // this is VERY costly, ideally the CPU should do it instead
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
}