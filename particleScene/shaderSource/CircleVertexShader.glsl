#version 410 core

layout(location = 0) in vec4 inPosition;

uniform vec4 cursorPos;
uniform mat4 projection;
uniform mat4 view;

out vec4 fragColor;

vec3 hsvToRgb(float h, float s, float v) {
    float r = abs(h * 6.0 - 3.0) - 1.0;
    float g = 2.0 - abs(h * 6.0 - 2.0);
    float b = 2.0 - abs(h * 6.0 - 4.0);
    return clamp(vec3(r, g, b), 0.0, 1.0) * v;
}

void main() {

    gl_Position = projection * view * inPosition;
    float distance = distance(gl_Position.xy, cursorPos.xy);
    float normalizedDistance = clamp(distance / sqrt(12.0), 0.0, 1.0); // Normalize distance

    float hue = normalizedDistance;
    float value = mix(1.0, 0.1, normalizedDistance);
    fragColor = vec4(hsvToRgb(hue, 1.0, value),1.0);
}