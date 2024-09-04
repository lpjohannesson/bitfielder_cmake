#pragma once
#include <vector>
#include "animation.h"

namespace bf {
    class AnimationSet {
    private:
        std::vector<Animation> animations;

    public:
        Animation *getAnimation(int index);
        void addAnimation(const std::vector<int> sequence, float duration = 0.0f, bool loops = true);
    };
}