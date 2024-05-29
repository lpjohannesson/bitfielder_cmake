#pragma once
#include <vector>
#include "sprite_frames.h"

namespace bf {
    class SpriteAnimation {
    public:
        SpriteFrames *frames;    
        std::vector<int> sequence;
        float duration = 0.0f;

        Box2 &getFrame(float time);

        void createAnimation(SpriteFrames &frames, const std::vector<int> &sequence, float duration = 0.0f);
    };
}