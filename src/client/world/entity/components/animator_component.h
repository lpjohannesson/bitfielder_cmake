#pragma once
#include "../animation/animation_set.h"

namespace bf {
	struct AnimatorComponent {
        AnimationSet *animations = nullptr;
		float time = 0.0f;
	};
}