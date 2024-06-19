#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
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