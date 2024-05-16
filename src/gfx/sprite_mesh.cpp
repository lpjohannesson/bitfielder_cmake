#include "sprite_mesh.h"
#include "sprite_renderer.h"
#include "sprite_vertex.h"
#include "texture.h"

using namespace bf;

void SpriteMesh::startAttributes() {
	// Position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void *)offsetof(SpriteVertex, position));
	glEnableVertexAttribArray(0);

	// UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void *)offsetof(SpriteVertex, uv));
	glEnableVertexAttribArray(1);
}

void SpriteMesh::start(const SpriteRenderer &renderer) {
	// Create objects
	glGenVertexArrays(1, &glVertexArray);
	glGenBuffers(1, &glVertexBuffer);

	// Bind objects
	glBindVertexArray(glVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, glVertexBuffer);

	// Bind renderer's index buffer, same for every mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.getGLIndexBuffer());

	// Assign vertex attrbutes
	startAttributes();

	// Unbind
	glBindVertexArray(0);
}

void SpriteMesh::end() {
	glDeleteVertexArrays(1, &glVertexArray);
	glDeleteBuffers(1, &glVertexBuffer);
}
