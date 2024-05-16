#pragma once
#include <GL/glew.h>
#include "sprite_mesh.h"
#include "texture.h"

namespace bf {
	class SpriteRenderer {
	private:
		GLuint glIndexBuffer, glProgram;

		void createIndexBuffer();
		void createShaderProgram();

	public:
		static constexpr int MAX_SPRITES = 2048;

		inline GLuint getGLIndexBuffer() const { return glIndexBuffer; }

		void renderMesh(const SpriteMesh &mesh, const Texture &texture);

		void start();
		void end();
	};
}