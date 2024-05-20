#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "sprite.h"
#include "sprite_mesh.h"

namespace bf {
	class SpriteBatch {
	private:
		std::vector<Sprite> sprites;

	public:
		void drawSprite(const Sprite &sprite);
		void uploadMesh(SpriteMesh &mesh);
	};
}