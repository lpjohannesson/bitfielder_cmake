#version 330 core

in vec2 fPosition;
in vec2 fUV;
in vec4 fColor;

out vec4 fOutColor;

uniform sampler2D fTexture;

void main() {
    vec4 color = texture(fTexture, fUV);

    if (color.a == 0.0) {
        discard;
    }

    fOutColor.r = 1.0;
}