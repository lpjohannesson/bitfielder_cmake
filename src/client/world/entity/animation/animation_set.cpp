#include "animation_set.h"

using namespace bf;

Animation *AnimationSet::getAnimation(int index) {
    if (index < 0 || index >= animations.size()) {
        return nullptr;
    }

    return &animations.at(index);
}

void AnimationSet::addAnimation(const std::vector<int> sequence, float duration, bool loops) {
    animations.emplace_back(sequence, duration, loops);
}