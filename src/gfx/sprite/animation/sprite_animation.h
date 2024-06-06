#pragma once
#include <vector>
#include "sprite_frames.h"

namespace bf {
    class SpriteAnimation {
    public:   
        std::vector<int> sequence;
        float duration;
        bool loops;

        Box2 getFrame(const SpriteFrames &frames, float time) const;

        SpriteAnimation(const std::vector<int> sequence, float duration, bool loops);
    };
}