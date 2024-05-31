#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
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
		
		void renderMesh(const SpriteMesh &mesh, const SpriteProgram &program) const;

		SpriteRenderer();
		~SpriteRenderer();
	};
}