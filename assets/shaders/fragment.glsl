#version 330 core

in vec2 fUV;
out vec4 fColor;

uniform sampler2D fTexture;

void main() {
    vec4 color = texture(fTexture, fUV);

    if (color.a == 0.0) {
        discard;
    }

    fColor = color;
}