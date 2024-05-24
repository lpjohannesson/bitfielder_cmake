#include "client_content.h"
#include "client/world/block/components/block_renderer_component.h"
#include "client/world/entity/components/sprite_component.h"
#include "client/world/entity/components/client_player_component.h"

using namespace bf;

void ClientContent::loadContent(World &world) {
	// Add entity systems
	world.entities.addSystem(clientPlayerSystem);

	// Load texture atlas
    std::vector<std::string> texturePaths = {
		"assets/textures/player.png",
		"assets/textures/tile.png",
	};

	world.renderer.textureAtlas.loadAtlas(texturePaths);

	// Load test block renderer
    testBlockRenderer.sprite.box.size = glm::vec2(1.0f);
    testBlockRenderer.sprite.uvBox = world.renderer.textureAtlas.getSection("assets/textures/tile.png").uvBox;

    world.blocks.registry.emplace<BlockRendererComponent>(world.content.testBlock, BlockRendererComponent { &testBlockRenderer });

	// Load player
	entt::registry &entityRegistry = world.entities.registry;

	entityRegistry.emplace<ClientPlayerComponent>(
		world.content.player, ClientPlayerComponent { });

	TextureSection playerTextureSection = world.renderer.textureAtlas.getSection("assets/textures/player.png");

	entityRegistry.emplace<SpriteComponent>(
		world.content.player, SpriteComponent { glm::vec2(playerTextureSection.size) / 16.0f, playerTextureSection.uvBox });
}