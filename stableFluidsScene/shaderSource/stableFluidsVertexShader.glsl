#version 410 core

layout(location = 0) in vec4 _position;
layout(location = 1) in vec2 _uv;

uniform mat4 projection;
uniform mat4 view;

out vec2 texCoord;

void main() {

    gl_Position = projection * view * _position;
    texCoord = _uv;
}