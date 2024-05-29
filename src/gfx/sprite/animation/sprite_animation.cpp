#include "sprite_animation.h"

using namespace bf;

Box2 SpriteAnimation::getFrame(const SpriteFrames &frames, float time) const {
    // Get frame index in sequence
    int sequenceIndex;

    if (duration == 0.0f) {
        sequenceIndex = 0;
    }
    else {
        sequenceIndex = glm::floor(time / duration * sequence.size());
    }

    // Get frame
    int frameIndex = sequence.at(sequenceIndex);
    return frames.frames.at(frameIndex);
}

SpriteAnimation::SpriteAnimation(const std::vector<int> sequence, float duration) {
    this->sequence = sequence;
    this->duration = duration;
}