#include "client_content.h"
#include "client/world/block/renderers/block_renderer.h"
#include "client/world/entity/components/sprite_component.h"

using namespace bf;

void ClientContent::loadContent(World &world) {
	// Load texture atlas
    std::vector<std::string> texturePaths = {
		"assets/textures/player.png",
		"assets/textures/tile.png",
	};

	world.renderer.textureAtlas.loadAtlas(texturePaths);

	// Load test block renderer
    testBlockRenderer.sprite.box.size = glm::vec2(1.0f);
    testBlockRenderer.sprite.uvBox = world.renderer.textureAtlas.getSection("assets/textures/tile.png").uvBox;

    world.blocks.registry.emplace<BlockRenderer*>(world.content.testBlock, &testBlockRenderer);

	// Load player sprite
	Sprite playerSprite;
	TextureSection playerTextureSection = world.renderer.textureAtlas.getSection("assets/textures/player.png");

	playerSprite.box.size = glm::vec2(playerTextureSection.size) / 16.0f;
	playerSprite.uvBox = playerTextureSection.uvBox;

	world.entities.registry.emplace<SpriteComponent>(
		world.entities.player, SpriteComponent { playerSprite });
}