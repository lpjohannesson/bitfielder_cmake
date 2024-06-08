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

int Font::createCharacterWidth(SDL_Surface *surface, glm::ivec2 start) {
    // Get maximum width of character
    int characterWidth = 0;

    for (int y = 0; y < frameSize.y; y++) {
        for (int x = characterWidth; x < frameSize.x; x++) {
            glm::ivec2 pixelPosition = start + glm::ivec2(x, y);
            glm::vec4 color = Color::getSurfacePixel(surface, pixelPosition);

            if (color.a == 0.0f) {
                continue;
            }

            characterWidth = x;
        }
    }

    return characterWidth;
}

void Font::drawText(std::string text, SpriteBatch &spriteBatch, glm::vec2 position) {
    Sprite sprite;

    sprite.box.start.y = position.y;
    sprite.box.size = glm::vec2(frameSize) / 16.0f;

    float characterX = 0;

    for (int i = 0; i < text.length(); i++) {
        // Draw character
        sprite.box.start.x = position.x + characterX / 16.0f;

        int characterIndex = getCharacterIndex(text.at(i));
        sprite.uvBox = frames.frames.at(characterIndex);

        spriteBatch.drawSprite(sprite);

        // Move forward
        characterX += characterWidths[characterIndex];
    }
}

void Font::loadFont(SDL_Surface *surface, TextureSection section, int spaceWidth) {
    frames.loadFrames(section.uvBox, FRAME_COUNTS);

    // Calculate character widths
    frameSize = section.box.size / FRAME_COUNTS;

    for (int y = 0; y < FRAME_COUNTS.y; y++) {
        for (int x = 0; x < FRAME_COUNTS.x; x++) {
            int index = y * FRAME_COUNTS.x + x;
            int characterWidth = createCharacterWidth(surface, section.box.start + glm::ivec2(x, y) * frameSize);

            characterWidths[index] = characterWidth;
        }
    }

    // Set space width
    characterWidths[getCharacterIndex(' ')] = spaceWidth;
}