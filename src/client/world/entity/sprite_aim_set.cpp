#include "sprite_aim_set.h"

using namespace bf;

void SpriteAimSet::loadSprite(SpriteSet &sprites, const std::string path, glm::ivec2 counts, const TextureAtlas &textureAtlas) {
    sprites.loadSprites(textureAtlas.getSection(path).uvBox, counts);
}

SpriteSet &SpriteAimSet::getFromAim(int aim) {
    switch (aim) {
    case -1:
        return up;
    case 1:
        return down;
    default:
        return forward;
    }
}

void SpriteAimSet::loadSprites(const std::string path, glm::ivec2 counts, const TextureAtlas &textureAtlas) {
    loadSprite(forward, path + "/forward.png", counts, textureAtlas);
    loadSprite(up, path + "/up.png", counts, textureAtlas);
    loadSprite(down, path + "/down.png", counts, textureAtlas);
}