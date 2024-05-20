#pragma once
#include <glm/glm.hpp>

namespace bf {
	class Scene {
	public:
		inline virtual void updateSize(glm::ivec2 size) {}

		inline virtual void update() {}
		inline virtual void render() {}

		inline virtual void start() {}
		inline virtual void end() {}
	};
}