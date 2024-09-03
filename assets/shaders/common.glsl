#version 300 es

precision mediump float;

vec4 getSpriteBaseColor(float fSpriteIndex, vec2 fColorPosition, sampler2D fColorTexture) {
    float colorWidth = 1.0 / float(textureSize(fColorTexture, 0).x);
    float colorStart = fSpriteIndex * 4.0 * colorWidth;

    vec4
        topLeftColor = texture(fColorTexture, vec2(colorStart, 0)),
        topRightColor = texture(fColorTexture, vec2(colorStart + colorWidth, 0)),
        bottomLeftColor = texture(fColorTexture, vec2(colorStart + colorWidth * 2.0, 0)),
        bottomRightColor = texture(fColorTexture, vec2(colorStart + colorWidth * 3.0, 0)),

        topColor = mix(topLeftColor, topRightColor, fColorPosition.x),
        bottomColor = mix(bottomLeftColor, bottomRightColor, fColorPosition.x),
        color = mix(topColor, bottomColor, fColorPosition.y);

    return glm::vec4(1.0) - color;
}

vec4 getSpriteColor(float fSpriteIndex, vec2 fUV, vec2 fColorPosition, sampler2D fColorTexture, sampler2D fTexture) {
    return texture(fTexture, fUV) - getSpriteBaseColor(fSpriteIndex, fColorPosition, fColorTexture);
}