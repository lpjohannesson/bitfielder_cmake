#pragma once
#include <glm/glm.hpp>

namespace bf {
	struct LocalPlayerComponent {
		glm::vec2 lastPosition{};
		bool lastSpriteFlip = false;

		bool jumpStopped = false;
		float floorTime = 0.0f, jumpTime = 0.0f, blockTime = 0.0f, blockTweenTime = 0.0f;
		glm::vec2 blockTweenStart, blockTweenEnd;

		int selectedBlockIndex = 1;
	};
}