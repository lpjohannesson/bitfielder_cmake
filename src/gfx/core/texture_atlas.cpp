#include "texture_atlas.h"
#include <filesystem>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <algorithm> 

using namespace bf;

int TextureAtlas::roundToTwoPower(int n) {
	return (int)glm::pow(2.0f, glm::ceil(glm::log(n) / glm::log(2.0f)));
}

TextureSection TextureAtlas::getSection(const std::string path) const {
	if (!std::filesystem::exists(path)) {
		return {};
	}

	std::string canonicalPath = std::filesystem::canonical(path).string();

	return sections.at(canonicalPath);
}

SDL_Surface *TextureAtlas::loadSurface(const std::vector<std::string> &paths) {
	sections.clear();

	std::vector<TextureAtlasEntry> entries;
	
	glm::ivec2 atlasSize = { MIN_WIDTH, 0 };

	for (const std::string &path : paths) {
		std::string canonicalPath = std::filesystem::canonical(path).string();

		// Load image
		SDL_Surface *imageSurface = IMG_Load(canonicalPath.c_str());
		SDL_ConvertSurfaceFormat(imageSurface, SDL_PIXELFORMAT_ARGB32, 0);

		if (imageSurface == nullptr) {
			std::cout << "Texture \"" << path << "\" could not be found." << std::endl;
			continue;
		}

		// Add texture
		glm::ivec2 size = { imageSurface->w, imageSurface->h };
		Box2i box = { glm::ivec2(0), size };

		// Expand atlas width
		if (atlasSize.x < size.x) {
			atlasSize.x = roundToTwoPower(size.x);
		}

		// Add to dictionary
		entries.push_back({ canonicalPath, imageSurface, box });
	}

	// Sort entries by area
	std::sort(entries.begin(), entries.end(),
		[](const TextureAtlasEntry &entry1, const TextureAtlasEntry &entry2) {
			const Box2i 
				&box1 = entry1.box,
				&box2 = entry2.box;

			return box1.getArea() > box2.getArea();
		});

	// Move boxes
	for (int entryIndex = 0; entryIndex < entries.size(); entryIndex++) {
		TextureAtlasEntry &entry = entries[entryIndex];
		Box2i &box = entry.box;

		while (true) {
			// Check for previous overlapping textures
			bool valid = true;

			for (int otherEntryIndex = 0; otherEntryIndex < entryIndex; otherEntryIndex++) {
				TextureAtlasEntry &otherEntry = entries[otherEntryIndex];
				Box2i &otherBox = otherEntry.box;

				if (Box2i::overlaps(box, otherBox)) {
					valid = false;
					break;
				}
			}

			// End if not overlapping
			if (valid) {
				break;
			}

			// Move texture box by step
			box.start.x += CHECK_STEP.x;

			if (box.start.x + box.size.x > atlasSize.x) {
				box.start.x = 0;
				box.start.y += CHECK_STEP.y;
			}
		}

		// Expand atlas height
		int height = box.start.y + box.size.y;

		if (atlasSize.y < height) {
			atlasSize.y = roundToTwoPower(height);
		}
	}

	// Create atlas
	SDL_Surface *surface = SDL_CreateRGBSurface(0, atlasSize.x, atlasSize.y, 32,
		0x000000FF,
		0x0000FF00,
		0x00FF0000,
		0xFF000000);

	for (const TextureAtlasEntry &entry : entries) {
		// Blit texture
		const Box2i &box = entry.box;

		SDL_Rect rect{};

		rect.x = box.start.x;
		rect.y = box.start.y;
		rect.w = box.size.x;
		rect.h = box.size.y;

		SDL_BlitSurface(entry.imageSurface, NULL, surface, &rect);
		SDL_FreeSurface(entry.imageSurface);

		// Create section and boxes
		TextureSection section;

		section.box = box;

		glm::vec2 atlasSizeF = atlasSize;
		section.uvBox = { glm::vec2(box.start) / atlasSizeF, glm::vec2(box.size) / atlasSizeF };

		// Map section to image path
		sections.emplace(entry.path, section);
	}

	return surface;
}