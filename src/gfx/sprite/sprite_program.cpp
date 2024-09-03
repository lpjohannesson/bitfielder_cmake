#include "sprite_program.h"
#include <glm/gtc/type_ptr.hpp>
#include "gfx/core/shader.h"

using namespace bf;

void SpriteProgram::setTransform(glm::mat4 transform) {
	glUseProgram(glProgram);
	glUniformMatrix4fv(glTransformLocation, 1, GL_FALSE, glm::value_ptr(transform));
}

void SpriteProgram::assignTexture(int index, const char *name) {
	GLint location = glGetUniformLocation(glProgram, name);

	glUseProgram(glProgram);
	glUniform1i(location, index);
}

void SpriteProgram::attachShader(GLuint shader) {
	glAttachShader(glProgram, shader);
}

void SpriteProgram::link() {
	Shader::linkProgram(glProgram);

	glTransformLocation = glGetUniformLocation(glProgram, "transform");
	assignTexture(0, "fColorTexture");
}

SpriteProgram::SpriteProgram() {
	glProgram = glCreateProgram();
}

SpriteProgram::~SpriteProgram() {
	glDeleteProgram(glProgram);
}
