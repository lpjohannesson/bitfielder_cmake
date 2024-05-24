#pragma once
#include <glm/glm.hpp>

namespace bf {
	struct BodyComponent {
		glm::vec2 velocity;
		glm::vec2 size;
	};
}