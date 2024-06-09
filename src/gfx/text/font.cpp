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

void Font::drawText(std::string text, FontProperties &properties) {
    Sprite sprite;

    sprite.box.start.y = properties.position.y;
    sprite.box.size = frameSize;
    sprite.color = properties.color;

    float offsetX;
    
    if (properties.centered) {
        float lineLength = 0;

        for (int i = 0; i < text.length(); i++) {
            lineLength += spacings[getCharacterIndex(text.at(i))];
        }

        offsetX = lineLength * -0.5f;
    }
    else {
        offsetX = 0.0f;
    }

    float spacing = 0.0f;

    for (int i = 0; i < text.length(); i++) {
        // Draw character
        sprite.box.start.x = offsetX + properties.position.x + spacing;

        int characterIndex = getCharacterIndex(text.at(i));
        sprite.uvBox = frames.frames.at(characterIndex);

        properties.spriteBatch->drawSprite(sprite);

        // Move forward
        spacing += spacings[characterIndex];
    }
}

void Font::loadFont(TextureSection section) {
    frames.loadFrames(section.uvBox, FRAME_COUNTS);
    frameSize = section.box.size / FRAME_COUNTS;
}