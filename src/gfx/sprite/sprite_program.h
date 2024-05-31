#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace bf {
	class SpriteProgram {
	private:
		GLuint glProgram;

	public:
        inline GLuint getGLProgram() const { return glProgram; }

        void setTransform(glm::mat4 transform);

        SpriteProgram(const char *vertexPath, const char *fragmentPath);
        ~SpriteProgram();
	};
}