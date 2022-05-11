#version 330 core

uniform float id;

in float depth;

out vec4 outId;

void main() {
    // red channel = id read
    // green channel = draw indicator
    // blue channel = depth normalized to max depth
    // alpha = unused
    outId = vec4(id, 1, -depth / 100.0f, 0);
}