#include "font.h"
#include <SDL2/SDL_image.h>
#include "core/color.h"

using namespace bf;

int Font::getCharacterIndex(char c) {
    // Check for unknown characters
    if (c < ' ' || c > '~') {
        return getCharacterIndex('?');
    }

    return c - 32;
}

float Font::getTextLength(std::string text, int end) const {
    float lineLength = 0;

    for (int i = 0; i < end; i++) {
        lineLength += spacings[getCharacterIndex(text.at(i))];
    }

    return lineLength;
}

glm::vec2 Font::getRenderPosition(std::string text, FontProperties &properties) const {
    float offsetX;
    
    if (properties.centered) {
        float lineLength = getTextLength(text, (int)text.length());
        offsetX = lineLength * -0.5f;
    }
    else {
        offsetX = 0.0f;
    }

    glm::vec2 renderPosition = properties.position;
    renderPosition.x += offsetX;

    return renderPosition;
}

void Font::drawText(std::string text, FontProperties &properties, SpriteBatch &spriteBatch) {
    Sprite letterSprite;

    letterSprite.box.start.y = properties.position.y;
    letterSprite.box.size = frameSize;
    letterSprite.setColor(properties.color);

    letterSprite.box.start = getRenderPosition(text, properties);

    float spacing = 0.0f;

    for (int i = 0; i < text.length(); i++) {
        // Draw character
        int characterIndex = getCharacterIndex(text.at(i));
        letterSprite.uvBox = frames.frames.at(characterIndex);

        Sprite &sprite = spriteBatch.createSprite();
        sprite = letterSprite;

        // Move forward
        letterSprite.box.start.x += spacings[characterIndex];
    }
}

void Font::loadFont(TextureSection section) {
    frames.loadFrames(section.uvBox, FRAME_COUNTS);
    frameSize = section.box.size / FRAME_COUNTS;
}