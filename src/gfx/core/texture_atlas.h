#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "core/box2.h"

namespace bf {
	class TextureSection {
	public:
		Box2i box;
		Box2 uvBox;
	};

	class TextureAtlasEntry {
	public:
		std::string path;
		SDL_Surface *imageSurface;
		Box2i box;
	};

	class TextureAtlas {
	private:
		static constexpr glm::ivec2 CHECK_STEP = { 8, 8 };
		static constexpr int MIN_WIDTH = 256;

		std::unordered_map<std::string, TextureSection> sections;

		static int roundToTwoPower(int n); 

	public:
		TextureSection getSection(const std::string path) const;

		SDL_Surface *loadSurface(const std::vector<std::string> &paths);
	};
}