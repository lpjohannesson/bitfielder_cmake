#version 330 core

in vec2 fUV;
out vec4 fColor;

uniform sampler2D fTexture;

void main() {
    fColor = texture(fTexture, fUV);
}