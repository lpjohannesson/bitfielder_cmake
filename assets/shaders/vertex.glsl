#version 300 es

precision mediump float;

layout (location = 0) in float vSpriteIndex;
layout (location = 1) in vec3 vPosition;
layout (location = 2) in vec2 vUV;
layout (location = 3) in vec2 vColorPosition;

flat out float fSpriteIndex;
out vec2 fPosition;
out vec2 fUV;
out vec2 fColorPosition;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(vPosition, 1.0);
    
    fSpriteIndex = vSpriteIndex;
    fPosition = (gl_Position.xy + 1.0) * 0.5;
    fUV = vUV;
    fColorPosition = vColorPosition;
}