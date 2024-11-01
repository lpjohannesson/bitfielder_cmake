#pragma once
#include <glm/glm.hpp>
#include "core/box2.h"

namespace bf {
	class Sprite {
	public:
		Box2 box, uvBox;
		float depth = 1.0f;

		glm::vec4 cornerColors[4];

		inline glm::vec4 &getTopLeftColor() { return cornerColors[0]; }
		inline glm::vec4 &getTopRightColor() { return cornerColors[1]; }
		inline glm::vec4 &getBottomLeftColor() { return cornerColors[2]; }
		inline glm::vec4 &getBottomRightColor() { return cornerColors[3]; }

		void setColor(glm::vec4 color);

		Sprite();
	};
}