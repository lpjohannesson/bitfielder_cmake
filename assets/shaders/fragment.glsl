#version 300 es

precision mediump float;

in vec2 fPosition;
in vec2 fUV;
in vec4 fColor;

out vec4 fOutColor;

uniform sampler2D fTexture;

void main() {
    vec4 color = texture(fTexture, fUV) - fColor;

    if (color.a == 0.0) {
        discard;
    }

    fOutColor.rgb = color.rgb;
}