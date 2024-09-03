#include "direction.h"

using namespace bf;

int Direction::posmod(int a, int b) {
    int m = a % b;

    if (m < 0) {
        return m + b;
    }

    return m;
}

void Direction::targetAxis(float &axis, float target, float change) {
    if (axis < target) {
        axis = glm::min(target, axis + change);
    }
    else {
        axis = glm::max(target, axis - change);
    }
}

int Direction::roundToTwoPower(int n) {
	return (int)glm::pow(2.0f, glm::ceil(glm::log(n) / glm::log(2.0f)));
}