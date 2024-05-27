#include "direction.h"

using namespace bf;

void Direction::targetAxis(float &axis, float target, float change) {
    if (axis < target) {
        axis = glm::min(target, axis + change);
    }
    else {
        axis = glm::max(target, axis - change);
    }
}