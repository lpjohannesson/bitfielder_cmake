#pragma once
#include <glm/glm.hpp>
#include "core/box2.h"

namespace bf {
	struct SpriteComponent {
		glm::vec2 size{};
        Box2 uvBox{};
		bool flipX = false;
	};
}