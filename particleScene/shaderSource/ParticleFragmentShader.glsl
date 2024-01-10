#version 410 core

in vec4 fragColor;
out vec4 color;
uniform uint shape;

void main() {
    float dist = length(gl_PointCoord - vec2(0.5, 0.5));
    if (shape == 1 && dist > 0.5) {
        discard;
    }
    color = fragColor;
}