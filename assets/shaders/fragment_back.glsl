#version 330 core

in vec2 fPosition;
in vec2 fUV;
out vec4 fColor;

uniform sampler2D fTexture;
uniform sampler2D fShadowTexture;

void main() {
    vec4 color = texture(fTexture, fUV);

    if (color.a == 0.0) {
        discard;
    }

    if (texture(fShadowTexture, fPosition).r > 0.0) {
        fColor.rgb = color.rgb - 0.5;
    }
    else {
        fColor.rgb = color.rgb - 0.3;
    }
}