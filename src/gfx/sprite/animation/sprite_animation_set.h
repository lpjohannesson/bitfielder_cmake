#pragma once
#include <vector>
#include "sprite_animation.h"
#include "../sprite_frames.h"

namespace bf {
    class SpriteAnimationSet {
    private:
        std::vector<SpriteAnimation> animations;

    public:
        SpriteAnimation *getAnimation(int index) const;
        void addAnimation(const std::vector<int> frames, float duration = 0.0f, bool loops = true);

        void getFrame(const SpriteFrames &frames, const SpriteAnimation &animation, float time, Box2 &result) const;
    };
}