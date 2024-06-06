#pragma once
#include "../components/sprite_aim_component.h"

namespace bf {
    class SpriteAimSystem {
    public:
        static SpriteFrames *getAimFrames(const SpriteAimComponent &spriteAim, int aim);
    };
}