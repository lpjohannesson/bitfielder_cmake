#include "client_content.h"
#include "client/world/block/renderers/block_renderer.h"
#include "client/world/entity/components/sprite_component.h"

using namespace bf;

void ClientContent::loadContent(World &world) {
	// Load texture atlas
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

	world.renderer.textureAtlas.loadAtlas(texturePaths);

	// Load test block renderer
    testBlockRenderer.sprite.box.size = glm::vec2(1.0f);
    testBlockRenderer.sprite.uvBox = world.renderer.textureAtlas.getSection("assets/textures/3.png").uvBox;

    world.blocks.registry.emplace<BlockRenderer*>(world.content.testBlock, &testBlockRenderer);

	// Load player sprite
	Sprite playerSprite;
	TextureSection playerTextureSection = world.renderer.textureAtlas.getSection("assets/textures/6.png");

	playerSprite.box.size = glm::vec2(playerTextureSection.size) / 16.0f;
	playerSprite.uvBox = playerTextureSection.uvBox;

	world.entities.registry.emplace<SpriteComponent>(
		world.entities.player, SpriteComponent { playerSprite });
}