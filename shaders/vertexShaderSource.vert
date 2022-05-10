#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

struct Warp {
    int flagWarp;
    float warpBy;
};

uniform Warp warp;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 PVM;

void main() {
    gl_Position = PVM * vec4(aPos, 1.0f);
    if (warp.flagWarp == 1) {
        gl_Position.x += sin(gl_Position.x) * warp.warpBy;
    }
    FragPos = vec3(model * vec4(aPos, 1.0));
    // gets rid of offsets caused by non-uniform scaling
    // this is VERY costly, ideally the CPU should do it instead
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
}