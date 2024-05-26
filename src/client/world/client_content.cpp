#include "client_content.h"
#include "client/scenes/world_scene.h"
#include "block/components/block_renderer_component.h"
#include "entity/components/sprite_component.h"
#include "entity/components/client_player_component.h"

using namespace bf;

entt::entity ClientContent::spawnRemotePlayer(WorldScene &scene, glm::vec2 position) {
	World &world = scene.world;
	WorldRenderer &worldRenderer = scene.worldRenderer;
	entt::registry &entityRegistry = world.entities.registry;

	entt::entity player = world.content.spawnPlayer(world, position);
	
	// Load sprite
	TextureSection playerTextureSection = worldRenderer.textureAtlas.getSection("assets/textures/player.png");

	entityRegistry.emplace<SpriteComponent>(
		player, SpriteComponent { glm::vec2(playerTextureSection.size) / 16.0f, playerTextureSection.uvBox });

	return player;
}

entt::entity ClientContent::spawnLocalPlayer(WorldScene &scene, glm::vec2 position) {
	World &world = scene.world;
	entt::registry &entityRegistry = world.entities.registry;

	// Spawn remote player with extra attributes
	entt::entity player = spawnRemotePlayer(scene, position);

	entityRegistry.emplace<ClientPlayerComponent>(
		player, ClientPlayerComponent { });
	
	entityRegistry.emplace<BodyComponent>(player, BodyComponent { glm::vec2(14.0f, 14.0f) / 16.0f });

	return player;
}

void ClientContent::loadContent(WorldScene &scene) {
	World &world = scene.world;
	WorldRenderer &worldRenderer = scene.worldRenderer;

	// Add entity systems
	world.entities.addSystem(clientPlayerSystem);

	// Load texture atlas
    std::vector<std::string> texturePaths = {
		"assets/textures/player.png",
		"assets/textures/tile.png",
	};

	worldRenderer.textureAtlas.loadAtlas(texturePaths);

	// Load test block renderer
    testBlockRenderer.sprite.box.size = glm::vec2(1.0f);
    testBlockRenderer.sprite.uvBox = worldRenderer.textureAtlas.getSection("assets/textures/tile.png").uvBox;

    world.blocks.registry.emplace<BlockRendererComponent>(world.content.testBlock, BlockRendererComponent { &testBlockRenderer });

	spawnLocalPlayer(scene, { 0.0f, 0.0f });
}