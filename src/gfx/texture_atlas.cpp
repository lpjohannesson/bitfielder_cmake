#include "texture_atlas.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <algorithm> 

using namespace bf;

int TextureAtlas::roundToTwoPower(int n) {
	return (int)glm::pow(2.0f, glm::ceil(glm::log(n) / glm::log(2.0f)));
}

void TextureAtlas::loadAtlas(const std::vector<std::string> &paths) {
	std::vector<TextureAtlasEntry> entries;
	
	glm::ivec2 atlasSize = { MIN_WIDTH, 0 };

	for (const std::string &path : paths) {
		// Load image
		SDL_Surface *surface = IMG_Load(path.c_str());

		if (surface == nullptr) {
			std::cout << "Texture \"" << path << "\" could not be found." << std::endl;
			continue;
		}

		// Add texture
		glm::ivec2 size = { surface->w, surface->h };
		Box2i box = { glm::ivec2(0), size };

		entries.push_back({ path, surface, box });

		// Expand atlas width
		if (atlasSize.x < size.x) {
			atlasSize.x = roundToTwoPower(size.x);
		}
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
	SDL_Surface *atlasSurface = SDL_CreateRGBSurface(0, atlasSize.x, atlasSize.y, 32,
		0x000000FF,
		0x0000FF00,
		0x00FF0000,
		0xFF000000);

	for (const TextureAtlasEntry &entry : entries) {
		// Blit texture
		const Box2i &box = entry.box;
		glm::ivec2 start = box.start, size = box.size;

		SDL_Rect rect{};

		rect.x = start.x;
		rect.y = start.y;
		rect.w = size.x;
		rect.h = size.y;

		SDL_BlitSurface(entry.surface, NULL, atlasSurface, &rect);
		SDL_FreeSurface(entry.surface);

		// Map section to entry data
		TextureSection section;
		section.size = box.size;

		// Normalize UV box
		glm::vec2 atlasSizeF = atlasSize;
		section.uvBox = { glm::vec2(start) / atlasSizeF, glm::vec2(size) / atlasSizeF };

		sections.emplace(entry.path, section);
	}

	IMG_SavePNG(atlasSurface, "atlas.png");

	// Upload atlas to texture
	texture.loadSurface(atlasSurface);
	SDL_FreeSurface(atlasSurface);
}

TextureSection TextureAtlas::getSection(const std::string path) const {
	return sections.at(path);
}

void TextureAtlas::start() {
	texture.start();
}

void TextureAtlas::end() {
	texture.end();
}
