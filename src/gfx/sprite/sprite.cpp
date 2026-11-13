#include "sprite.h"

using namespace bf;

void Sprite::setColor(glm::vec4 color) {
    topLeftColor =
        topRightColor =
        bottomLeftColor =
        bottomRightColor =
        color;
}

Sprite::Sprite() {
    setColor(glm::vec4(1.0f));
}