#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "sprite.h"
#include "sprite_mesh.h"

namespace bf {
	class SpriteBatch {
	private:
		std::vector<Sprite> sprites;
		
		void addColor(glm::vec4 color, std::vector<uint8_t> &data);

	public:
		float defaultDepth = 1.0f;

		Sprite &createSprite();
		void uploadMesh(SpriteMesh &mesh);
	};
}