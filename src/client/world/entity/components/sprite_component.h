#pragma once
#include <glm/glm.hpp>
#include "core/box2.h"

namespace bf {
	struct SpriteComponent {
		glm::vec2 size{}, offset{};
        Box2 uvBox{};
		glm::vec4 color{ 1.0f };
	};
}