#version 330 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vUV;
layout (location = 2) in vec4 vColor;

out vec2 fPosition;
out vec2 fUV;
out vec4 fColor;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(vPosition, 0.0, 1.0);
    
    fPosition = (gl_Position.xy + 1.0) * 0.5;
    fUV = vUV;
    fColor = vec4(1.0f) - vColor;
}