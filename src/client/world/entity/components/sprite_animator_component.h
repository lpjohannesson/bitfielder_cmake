#pragma once
#include "gfx/sprite/animation/sprite_animation_set.h"

namespace bf {
	struct SpriteAnimatorComponent {
		SpriteFrames *frames = nullptr;
        SpriteAnimationSet *animationSet = nullptr;
		float time = 0.0f;
	};
}