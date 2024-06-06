#pragma once
#include "gfx/sprite/animation/sprite_frames.h"

namespace bf {
    struct SpriteAimComponent {
        SpriteFrames *forwardFrames, *upFrames, *downFrames;
    };
}