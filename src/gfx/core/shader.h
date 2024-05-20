#pragma once
#include <GL/glew.h>

namespace bf {
	class Shader {
	public:
		static void compileShader(GLuint glShader, const char *path);
		static void linkProgram(GLuint glProgram);
	};
}