#include "animation.h"
#include <glm/glm.hpp>

using namespace bf;

int Animation::getFrame(float time) const {
    // Get frame index in sequence
    int sequenceIndex;

    if (duration == 0.0f) {
        sequenceIndex = 0;
    }
    else {
        int timeIndex = (int)glm::floor(time / duration * sequence.size());
        sequenceIndex = glm::min((int)sequence.size() - 1, timeIndex);
    }

    // Get frame
    return sequence.at(sequenceIndex);
}

Animation::Animation(const std::vector<int> sequence, float duration, bool loops) {
    this->sequence = sequence;
    this->duration = duration;
    this->loops = loops;
}