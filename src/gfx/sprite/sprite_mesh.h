#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace bf {
	class SpriteRenderer;

	struct SpriteVertex {
		glm::vec2 position;
		glm::vec2 uv;
		glm::vec4 color;
	};

	class SpriteMesh {
	private:
		GLuint glVertexArray, glVertexBuffer;

		void startAttributes();

	public:
		int elementCount = 0;

		inline GLuint getGLVertexArray() const { return glVertexArray; }
		inline GLuint getGLVertexBuffer() const { return glVertexBuffer; }

		SpriteMesh(const SpriteRenderer &renderer);
		~SpriteMesh();
	};
}