#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "sprite_mesh.h"
#include "sprite_program.h"
#include "gfx/core/texture.h"

namespace bf {
	class SpriteRenderer {
	private:
		GLuint glIndexBuffer;

		void createIndexBuffer();

	public:
		static constexpr int MAX_SPRITES = 2048;

		inline GLuint getGLIndexBuffer() const { return glIndexBuffer; }
		
		void renderMesh(const SpriteMesh &mesh, const SpriteProgram &program, const Texture &texture) const;

		SpriteRenderer();
		~SpriteRenderer();
	};
}