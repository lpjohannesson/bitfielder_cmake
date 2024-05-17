#include "sprite_batch.h"
#include <glm/glm.hpp>
#include "sprite_renderer.h"

using namespace bf;

void SpriteBatch::drawSprite(const Sprite &sprite) {
	sprites.push_back(sprite);
}

void SpriteBatch::uploadMesh(SpriteMesh &mesh) {
	// Limit sprite count
	int spriteCount = glm::min(SpriteRenderer::MAX_SPRITES, (int)sprites.size());
	int vertexCount = spriteCount * 4;

	// Store vertices for all sprites
	std::vector<SpriteVertex> vertices;
	vertices.resize(vertexCount);

	for (int spriteIndex = 0; spriteIndex < spriteCount; spriteIndex++) {
		const Sprite &sprite = sprites[spriteIndex];

		// Get corners
		glm::vec2
			start = sprite.box.start,
			end = sprite.box.getEnd(),
			uvStart = sprite.uvBox.start,
			uvEnd = sprite.uvBox.getEnd();

		// Push vertices
		int vertexStart = spriteIndex * 4;

		vertices[vertexStart + 0] = { start, uvStart };
		vertices[vertexStart + 1] = { { end.x, start.y }, { uvEnd.x, uvStart.y } };
		vertices[vertexStart + 2] = { { start.x, end.y }, { uvStart.x, uvEnd.y } };
		vertices[vertexStart + 3] = { end, uvEnd };
	}

	// Upload mesh
	glBindBuffer(GL_ARRAY_BUFFER, mesh.getGLVertexBuffer());
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertex) * vertexCount, vertices.data(), GL_STATIC_DRAW);
	
	mesh.elementCount = spriteCount * 6;

	// Clear sprite list
	sprites.clear();
}