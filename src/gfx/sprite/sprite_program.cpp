#include "sprite_program.h"
#include <glm/gtc/type_ptr.hpp>
#include "gfx/core/shader.h"

using namespace bf;

void SpriteProgram::setTransform(glm::mat4 transform) {
	glUseProgram(glProgram);
	
	GLint glTransformLocation = glGetUniformLocation(glProgram, "transform");
	glUniformMatrix4fv(glTransformLocation, 1, GL_FALSE, glm::value_ptr(transform));
}

SpriteProgram::SpriteProgram(const char *vertexPath, const char *fragmentPath) {
	glProgram = glCreateProgram();

    // Create shaders
    GLuint glVertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint glFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	Shader::compileShader(glVertexShader, vertexPath);
    Shader::compileShader(glFragmentShader, fragmentPath);

    glAttachShader(glProgram, glVertexShader);
    glAttachShader(glProgram, glFragmentShader);

    Shader::linkProgram(glProgram);

	// Delete shaders
	glDeleteShader(glVertexShader);
	glDeleteShader(glFragmentShader);
}

SpriteProgram::~SpriteProgram() {
	glDeleteProgram(glProgram);
}
