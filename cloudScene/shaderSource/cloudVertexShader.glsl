#version 410 core

layout(location = 0) in vec4 _position;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _uv;
layout(location = 3) in vec3 _color;

uniform mat4 projection;
uniform mat4 view;

out vec3 incolor;

void main() {

    gl_Position = projection* view* _position;
    incolor = _color;
}