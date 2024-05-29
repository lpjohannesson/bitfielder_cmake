#include "sprite_animation.h"

using namespace bf;

Box2 &SpriteAnimation::getFrame(float time) {
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
    return frames->frames.at(frameIndex);
}

void SpriteAnimation::createAnimation(SpriteFrames &frames, const std::vector<int> &sequence, float duration) {
    this->frames = &frames;
    this->sequence = sequence;
    this->duration = duration;
}