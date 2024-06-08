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

void Font::drawText(std::string text, SpriteBatch &spriteBatch, glm::vec2 position) {
    Sprite sprite;

    sprite.box.start.y = position.y;
    sprite.box.size = glm::vec2(frameSize) / 16.0f;

    float spacing = 0;

    for (int i = 0; i < text.length(); i++) {
        // Draw character
        sprite.box.start.x = position.x + spacing / 16.0f;

        int characterIndex = getCharacterIndex(text.at(i));
        sprite.uvBox = frames.frames.at(characterIndex);

        spriteBatch.drawSprite(sprite);

        // Move forward
        spacing += spacings[characterIndex];
    }
}

void Font::loadFont(TextureSection section) {
    frames.loadFrames(section.uvBox, FRAME_COUNTS);
    frameSize = section.box.size / FRAME_COUNTS;
}