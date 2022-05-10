#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;

uniform mat4 PVM;
uniform float time;

out vec2 TexCoord;
float decay = 0.20f;

void main() {
    gl_Position = PVM * vec4(inPos, 1.0f);

    float localTime = time * decay;
    vec2 offset = vec2((floor(localTime) - localTime) * 4 + 1.0f, 0.0f);

    TexCoord = inTexCoord + offset;
}