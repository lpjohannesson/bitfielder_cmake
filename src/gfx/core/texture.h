#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <SDL2/SDL.h>

namespace bf {
	class Texture {
	private:
		GLuint glTexture;

	public:
		inline GLuint getGLTexture() const { return glTexture; }

		void loadSurface(SDL_Surface *surface);

		Texture();
		~Texture();
	};
}