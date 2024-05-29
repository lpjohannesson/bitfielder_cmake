#pragma once
#include "gfx/sprite/animation/sprite_animation.h"

namespace bf {
	struct SpriteAnimatorComponent {
        SpriteAnimation *animation = nullptr;
		float time = 0.0f;
	};
}