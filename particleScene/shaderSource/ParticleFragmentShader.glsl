#version 410 core

in vec4 fragColor;
out vec4 color;
uniform uint shape;
uniform sampler2D myTextureSampler;

void main() {
    float dist = length(gl_PointCoord - vec2(0.5, 0.5));
    if (shape == 1 && dist > 0.5) {
        discard;
    }
    // vec4 sprite = texture(myTextureSampler, gl_PointCoord );
    // color = sprite;
    color = fragColor;
}