#version 330 core

uniform float time;
uniform mat4 view;
uniform sampler2D texSampler;

in vec3 position;
in vec2 TexCoord;

out vec4 FragColor;

// 1 row x 25 frames
uniform ivec2 pattern = ivec2(25, 1);
uniform float frameDuration = 0.1f;

vec4 sampleTexture(int frame) {
    vec2 offset = vec2(1.0f) / vec2(pattern);

    vec2 texCoordBase = TexCoord / vec2(pattern);
    vec2 texCoord = texCoordBase + vec2((frame % pattern.x),
                                       (frame / pattern.x)) * offset;
    return texture(texSampler, texCoord);
}

void main() {
    int frame = int(time / frameDuration);

    FragColor = sampleTexture(frame);
}