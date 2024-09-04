#include "sprite_set.h"

using namespace bf;

void SpriteSet::loadSprites(Box2 uvBox, glm::ivec2 counts) {
    // Create frames, row by row
    Box2 box;
    box.size = uvBox.size / (glm::vec2)counts;

    for (int y = 0; y < counts.y; y++) {
        for (int x = 0; x < counts.x; x++) {
            glm::vec2 position = { x, y };

            box.start = uvBox.start + box.size * position;
            boxes.push_back(box);
        }
    }
}