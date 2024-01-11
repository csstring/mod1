#version 410 core

in vec2 texCoord;
out vec4 color;

uniform sampler2D myTextureSampler;

void main() {
    // color = vec4(1, 0, 0, 1.0);
    color = texture(myTextureSampler, texCoord);
}