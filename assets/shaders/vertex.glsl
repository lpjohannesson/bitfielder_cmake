#version 330 core

out vec2 fUV;

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vUV;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(vPosition, 0.0, 1.0);
    fUV = vUV;
}