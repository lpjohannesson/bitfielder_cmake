#include "sprite_animation_set.h"

using namespace bf;

SpriteAnimation *SpriteAnimationSet::getAnimation(int index) const {
    if (index < 0 || index >= animations.size()) {
        return nullptr;
    }

    return (SpriteAnimation*)&animations.at(index);
}

void SpriteAnimationSet::addAnimation(const std::vector<int> sequence, float duration) {
    animations.emplace_back(sequence, duration);
}

void SpriteAnimationSet::getFrame(const SpriteAnimation &animation, float time, Box2 &result) const {
    result = animation.getFrame(frames, time);
}