#include "sprite_frames.h"

using namespace bf;

void SpriteFrames::loadFrames(Box2 uvBox, glm::ivec2 frameCounts) {
    // Create frames, row by row
    Box2 frame;
    frame.size = uvBox.size / (glm::vec2)frameCounts;

    for (int y = 0; y < frameCounts.y; y++) {
        for (int x = 0; x < frameCounts.x; x++) {
            glm::vec2 position = { x, y };

            frame.start = uvBox.start + frame.size * position;
            frames.push_back(frame);
        }
    }
}