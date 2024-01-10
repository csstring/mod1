#version 410 core

in vec4 fragColor;
out vec4 color;

void main() {
    float dist = length(gl_PointCoord - vec2(0.5, 0.5));
    if (dist > 0.5) {
        discard;
    }
    color = fragColor;
}