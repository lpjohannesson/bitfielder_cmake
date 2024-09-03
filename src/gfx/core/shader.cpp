#include "shader.h"
#include <iostream>
#include <vector>
#include "core/file_loader.h"

using namespace bf;

void Shader::compileShader(GLuint glShader, const char *path) {
	// Get shader source from file
	std::string source;

	if (!FileLoader::loadText(path, source)) {
		std::cout << "Shader \"" << path << "\" could not be found." << std::endl;
	}

	const char *sourceCStr = source.c_str();
	int sourceLength = (int)source.size();

	// Compile
	glShaderSource(glShader, 1, &sourceCStr, &sourceLength);
	glCompileShader(glShader);

	// Check for errors
	GLint compileStatus;
	glGetShaderiv(glShader, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus != 0) {
		return;
	}

	// Print error
	int errorLength;
	glGetShaderiv(glShader, GL_INFO_LOG_LENGTH, &errorLength);

	std::vector<GLchar> error;
	error.resize(errorLength);

	glGetShaderInfoLog(glShader, errorLength, &errorLength, error.data());

	std::cout << "Shader compilation \"" << path << "\" failed: " << std::endl << error.data() << std::endl;
}

void Shader::linkProgram(GLuint glProgram) {
	// Link
	glLinkProgram(glProgram);

	// Check for errors
	GLint linkStatus;
	glGetProgramiv(glProgram, GL_LINK_STATUS, &linkStatus);

	if (linkStatus != 0) {
		return;
	}

	// Print error
	int errorLength;
	glGetProgramiv(glProgram, GL_INFO_LOG_LENGTH, &errorLength);

	std::vector<GLchar> error;
	error.resize(errorLength);

	glGetProgramInfoLog(glProgram, errorLength, &errorLength, error.data());

	std::cout << "Program link failed: " << std::endl << error.data() << std::endl;
}
