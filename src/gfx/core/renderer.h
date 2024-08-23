#pragma once
#include <glm/glm.hpp>

namespace bf {
	class Renderer {
	public:
		static void updateSize(glm::ivec2 size);
		static void clearScreen(glm::vec4 color);
	};
}