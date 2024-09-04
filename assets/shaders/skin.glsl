#version 300 es

precision mediump float;

in vec2 fUV;

out vec4 outColor;

uniform sampler2D fTexture;
uniform sampler2D fSkinTexture;

void main() {
    vec4 color = texture(fTexture, fUV);

    if (color.a == 0.0) {
        discard;
    }

    vec4 skinColor = texture(fSkinTexture, color.rg);

    outColor.rgb = skinColor.rgb;
}