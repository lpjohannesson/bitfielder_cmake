#version 300 es

precision mediump float;

flat in float fSpriteIndex;
in vec2 fColorPosition;

out vec4 outColor;

uniform sampler2D fColorTexture;

vec4 getSpriteBaseColor(float fSpriteIndex, vec2 fColorPosition, sampler2D fColorTexture);

void main() {
    outColor = getSpriteBaseColor(fSpriteIndex, floor(fColorPosition * 4.0) / 4.0, fColorTexture);
}