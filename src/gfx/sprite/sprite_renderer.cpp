#include "sprite_renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include "gfx/core/shader.h"

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

void SpriteRenderer::createShaderProgram() {
    // Create shaders
    GLuint glVertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint glFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	Shader::compileShader(glVertexShader, "assets/shaders/vertex.glsl");
    Shader::compileShader(glFragmentShader, "assets/shaders/fragment.glsl");

    // Create shader program
    glProgram = glCreateProgram();

    glAttachShader(glProgram, glVertexShader);
    glAttachShader(glProgram, glFragmentShader);

    Shader::linkProgram(glProgram);

	// Delete shaders
	glDeleteShader(glVertexShader);
	glDeleteShader(glFragmentShader);
}

void SpriteRenderer::renderMesh(const SpriteMesh &mesh, const Texture &texture, glm::mat4 transform) const {
	glUseProgram(glProgram);
	glBindVertexArray(mesh.getGLVertexArray());

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getGLTexture());

	// Set transform
	GLint glTransformLocation = glGetUniformLocation(glProgram, "transform");
	glUniformMatrix4fv(glTransformLocation, 1, GL_FALSE, glm::value_ptr(transform));

	// Draw
	glDrawElements(GL_TRIANGLES, mesh.elementCount, GL_UNSIGNED_INT, nullptr);
}

SpriteRenderer::SpriteRenderer() {
	createIndexBuffer();
	createShaderProgram();
}

SpriteRenderer::~SpriteRenderer() {
	glDeleteProgram(glProgram);
	glDeleteBuffers(1, &glIndexBuffer);
}
