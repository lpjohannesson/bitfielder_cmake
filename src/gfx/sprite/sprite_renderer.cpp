#include "sprite_renderer.h"

using namespace bf;

void SpriteRenderer::createIndexBuffer() {
	// Index storage, 6 per sprite, 2 tris each
	unsigned int indices[MAX_SPRITES * 6];

	for (int i = 0; i < MAX_SPRITES; i++) {
		// Find first index and vertex
		int startVertex = i * 4;
		int startIndex = i * 6;

		// Write indices with offsets
		indices[startIndex + 0] = startVertex + 0;
		indices[startIndex + 1] = startVertex + 1;
		indices[startIndex + 2] = startVertex + 2;
		indices[startIndex + 3] = startVertex + 1;
		indices[startIndex + 4] = startVertex + 2;
		indices[startIndex + 5] = startVertex + 3;
	}

	// Create and upload index buffer
	glGenBuffers(1, &glIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void SpriteRenderer::renderMesh(const SpriteMesh &mesh, const SpriteProgram &program) const {
	// Bind color texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.colorTexture.getGLTexture());

	glUseProgram(program.getGLProgram());

	glBindVertexArray(mesh.getGLVertexArray());

	glDrawElements(GL_TRIANGLES, mesh.elementCount, GL_UNSIGNED_INT, nullptr);
}

void SpriteRenderer::renderMesh(const SpriteMesh &mesh, const SpriteProgram &program, const Texture &texture) const {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture.getGLTexture());

	renderMesh(mesh, program);
}

SpriteRenderer::SpriteRenderer() {
	createIndexBuffer();
}

SpriteRenderer::~SpriteRenderer() {
	glDeleteBuffers(1, &glIndexBuffer);
}
