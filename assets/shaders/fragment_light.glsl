#version 300 es

precision mediump float;

in vec2 fPosition;
in vec2 fUV;
in vec4 fColor;

out vec4 fOutColor;

uniform sampler2D fTexture;

void main() {
    fOutColor.rgb = fColor.rgb;
}