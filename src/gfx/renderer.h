#pragma once
#include <glm/glm.hpp>

namespace bf {
	class Renderer {
	public:
		void updateSize(glm::ivec2 size);
		void clearScreen(glm::vec4 color);
	};
}