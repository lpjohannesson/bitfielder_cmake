#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "gfx/core/texture.h"

namespace bf {
	class SpriteRenderer;

	struct SpriteVertex {
		float spriteIndex;
		glm::vec2 position;
		glm::vec2 uv;
		glm::vec2 colorPosition;
	};

	class SpriteMesh {
	private:
		GLuint glVertexArray, glVertexBuffer;

		void startAttributes();

	public:
		int elementCount = 0;
		Texture colorTexture;

		inline GLuint getGLVertexArray() const { return glVertexArray; }
		inline GLuint getGLVertexBuffer() const { return glVertexBuffer; }

		SpriteMesh(const SpriteRenderer &renderer);
		~SpriteMesh();
	};
}