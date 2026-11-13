#pragma once
#include <glm/glm.hpp>
#include "core/box2.h"

namespace bf {
	struct SpriteComponent {
		glm::vec2 size{}, offset{};
		glm::vec4 color{ 1.0f };
        Box2 uvBox{};
	};
}