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
		Sprite &createSprite();
		void uploadMesh(SpriteMesh &mesh);
	};
}