#pragma once
#include <glm/glm.hpp>

namespace bf {
	struct LocalPlayerComponent {
		glm::vec2 lastPosition{};
		bool lastSpriteFlip = false;

		bool positionDirty = false, spriteAnimationDirty = false, spriteFlipDirty = false;

		bool jumpStopped = false;
		float floorTime = 0.0f, jumpTime = 0.0f, blockTime = 0.0f;
	};
}