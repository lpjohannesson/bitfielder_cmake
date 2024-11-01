#include "sprite.h"

using namespace bf;

void Sprite::setColor(glm::vec4 color) {
    for (glm::vec4 &cornerColor : cornerColors) {
        cornerColor = color;
    }
}

Sprite::Sprite() {
    setColor(glm::vec4(1.0f));
}