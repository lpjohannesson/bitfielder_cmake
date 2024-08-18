#pragma once
#include <glm/glm.hpp>

namespace bf {
	struct LocalPlayerComponent {
		glm::vec2 lastPosition{};
		bool lastOnSurface = true;

		float floorTime = 0.0f, jumpTime = 0.0f, blockTime = 0.0f, blockTweenTime = 0.0f;
		bool jumpStopped = false;
		glm::vec2 blockTweenStart, blockTweenEnd;
	};
}