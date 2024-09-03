#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "gfx/core/texture.h"

namespace bf {
	class SpriteProgram {
	private:
		GLuint glProgram, glTransformLocation;

	public:
        inline GLuint getGLProgram() const { return glProgram; }

        void setTransform(glm::mat4 transform);
		void assignTexture(int index, const char *name);

		void attachShader(GLuint shader);
		void link();

        SpriteProgram();
        ~SpriteProgram();
	};
}