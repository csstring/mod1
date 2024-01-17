#version 410 core

in vec3 incolor;
out vec4 outcolor;

void main() {
    outcolor = vec4(incolor,1.0f); 
}