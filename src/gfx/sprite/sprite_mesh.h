#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace bf {
	class SpriteRenderer;

	struct SpriteVertex {
		glm::vec2 position;
		glm::vec2 uv;
	};

	class SpriteMesh {
	private:
		GLuint glVertexArray, glVertexBuffer;

		void startAttributes();

	public:
		int elementCount;

		inline GLuint getGLVertexArray() const { return glVertexArray; }
		inline GLuint getGLVertexBuffer() const { return glVertexBuffer; }

		SpriteMesh(const SpriteRenderer &renderer);
		~SpriteMesh();
	};
}