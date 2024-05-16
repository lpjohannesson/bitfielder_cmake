#pragma once
#include <GL/glew.h>

namespace bf {
	class SpriteRenderer;

	class SpriteMesh {
	private:
		GLuint glVertexArray, glVertexBuffer;

		void startAttributes();

	public:
		int elementCount;

		inline GLuint getGLVertexArray() const { return glVertexArray; }
		inline GLuint getGLVertexBuffer() const { return glVertexBuffer; }

		void start(const SpriteRenderer &spriteRenderer);
		void end();
	};
}