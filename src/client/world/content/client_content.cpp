#include "client_content.h"
#include "client/world/block/renderers/block_renderer.h"

using namespace bf;

void ClientContent::loadContent(World &world, WorldRenderer &renderer) {
    std::vector<std::string> texturePaths = {
		"assets/textures/1.png",
		"assets/textures/2.png",
		"assets/textures/3.png",
		"assets/textures/4.png",
		"assets/textures/5.png",
		"assets/textures/6.png",
		"assets/textures/7.png",
		"assets/textures/8.png",
	};

	renderer.textureAtlas.loadAtlas(texturePaths);

    testBlockRenderer.sprite.box.size = glm::vec2(1.0f);
    testBlockRenderer.sprite.uvBox = renderer.textureAtlas.getSection("assets/textures/3.png").uvBox;

    world.blocks.registry.emplace<BlockRenderer*>(world.content.testBlock, &testBlockRenderer);
}