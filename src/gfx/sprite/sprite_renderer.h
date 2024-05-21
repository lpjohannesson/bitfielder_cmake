#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "sprite_mesh.h"
#include "gfx/core/texture.h"

namespace bf {
	class SpriteRenderer {
	private:
		GLuint glIndexBuffer, glProgram;

		void createIndexBuffer();
		void createShaderProgram();

	public:
		static constexpr int MAX_SPRITES = 2048;

		inline GLuint getGLIndexBuffer() const { return glIndexBuffer; }

		void setTransform(glm::mat4 transform);
		void renderMesh(const SpriteMesh &mesh, const Texture &texture) const;

		SpriteRenderer();
		~SpriteRenderer();
	};
}