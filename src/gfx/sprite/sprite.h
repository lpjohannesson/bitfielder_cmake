#pragma once
#include <glm/glm.hpp>
#include "core/box2.h"

namespace bf {
	class Sprite {
	public:
		Box2 box, uvBox;
		glm::vec4 color{ 1.0f };
	};
}