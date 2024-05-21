#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "engine/box2.h"
#include "texture.h"

namespace bf {
	class TextureSection {
	public:
		glm::ivec2 size;
		Box2 uvBox;
	};

	class TextureAtlasEntry {
	public:
		std::string path;
		SDL_Surface *surface;
		Box2i box;
	};

	class TextureAtlas {
	private:
		static constexpr glm::ivec2 CHECK_STEP = { 16, 16 };
		static constexpr int MIN_WIDTH = 256;

		std::unordered_map<std::string, TextureSection> sections;

		static int roundToTwoPower(int n);

	public:
		Texture texture;

		void loadAtlas(const std::vector<std::string> &paths);
		TextureSection getSection(const std::string path) const;
	};
}