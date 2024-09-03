#version 300 es

precision mediump float;

flat in float fSpriteIndex;
in vec2 fPosition;
in vec2 fUV;
in vec2 fColorPosition;

out vec4 fOutColor;

uniform sampler2D fColorTexture;
uniform sampler2D fTexture;
uniform sampler2D fShadowTexture;

vec4 getSpriteColor(float fSpriteIndex, vec2 fUV, vec2 fColorPosition, sampler2D fColorTexture, sampler2D fTexture);

void main() {
    vec4 color = getSpriteColor(fSpriteIndex, fUV, fColorPosition, fColorTexture, fTexture);

    if (color.a == 0.0) {
        discard;
    }

    if (texture(fShadowTexture, fPosition).r > 0.0) {
        fOutColor.rgb = color.rgb - 0.4;
    }
    else {
        fOutColor.rgb = color.rgb - 0.2;
    }
}