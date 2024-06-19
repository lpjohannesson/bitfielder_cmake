#include "renderer.h"
#include <glad/glad.h>

using namespace bf;

void Renderer::updateSize(glm::ivec2 size) {
	glViewport(0, 0, size.x, size.y);
}

void Renderer::clearScreen(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}
