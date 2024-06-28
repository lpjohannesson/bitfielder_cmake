#pragma once
#include <glm/glm.hpp>

namespace bf {
	struct BodyComponent {
		glm::vec2 size{};

		bool isOnFloor = false, isOnCeiling = false;
	};
}