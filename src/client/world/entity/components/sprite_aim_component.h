#pragma once
#include "gfx/sprite/sprite_frames.h"

namespace bf {
    struct SpriteAimComponent {
        SpriteFrames *forwardFrames, *upFrames, *downFrames;
    };
}