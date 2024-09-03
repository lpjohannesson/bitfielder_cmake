#include "sprite_batch.h"
#include <glm/glm.hpp>
#include "sprite_renderer.h"
#include "core/direction.h"

using namespace bf;

Sprite &SpriteBatch::createSprite() {
	sprites.emplace_back();
	return sprites.back();
}

void SpriteBatch::addColor(glm::vec4 color, std::vector<uint8_t> &data) {
	glm::ivec4 byteColor = glm::ivec4(color * 255.0f);

	data.push_back(byteColor.r);
	data.push_back(byteColor.g);
	data.push_back(byteColor.b);
	data.push_back(byteColor.a);
}

void SpriteBatch::uploadMesh(SpriteMesh &mesh) {
	// Limit sprite count
	int spriteCount = glm::min(SpriteRenderer::MAX_SPRITES, (int)sprites.size());
	int vertexCount = spriteCount * 4;

	// Store vertices for all sprites
	std::vector<SpriteVertex> vertices;
	vertices.reserve(vertexCount);

	// Store vertex colours in texture
	std::vector<uint8_t> colors;

	int colorsWidth = Direction::roundToTwoPower(vertexCount);
	colors.reserve(colorsWidth * 4);

	for (int spriteIndex = 0; spriteIndex < spriteCount; spriteIndex++) {
		const Sprite &sprite = sprites[spriteIndex];

		// Get corners, round UVs to avoid artifacts
		glm::vec2
			start = sprite.box.start,
			end = sprite.box.getEnd(),
			uvStart = sprite.uvBox.start,
			uvEnd = sprite.uvBox.getEnd() - glm::pow(2.0f, -12.0f);

		// Push vertices
		float spriteIndexF = (float)spriteIndex;

		vertices.push_back({ spriteIndexF, start, uvStart, { 0.0f, 0.0f } });
		vertices.push_back({ spriteIndexF, { end.x, start.y }, { uvEnd.x, uvStart.y }, { 1.0f, 0.0f } });
		vertices.push_back({ spriteIndexF, { start.x, end.y }, { uvStart.x, uvEnd.y }, { 0.0f, 1.0f } });
		vertices.push_back({ spriteIndexF, end, uvEnd, { 1.0f, 1.0f } });

		addColor(sprite.topLeftColor, colors);
		addColor(sprite.topRightColor, colors);
		addColor(sprite.bottomLeftColor, colors);
		addColor(sprite.bottomRightColor, colors);
	}

	// Upload mesh
	glBindBuffer(GL_ARRAY_BUFFER, mesh.getGLVertexBuffer());
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	
	mesh.elementCount = spriteCount * 6;

	// Upload colour texture
	glBindTexture(GL_TEXTURE_2D, mesh.colorTexture.getGLTexture());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, colorsWidth, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, colors.data());

	// Clear sprite list
	sprites.clear();
}