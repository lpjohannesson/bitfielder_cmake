#include "client_content.h"
#include "client/scenes/world_scene.h"
#include "block/components/block_renderer_component.h"
#include "entity/components/sprite_component.h"
#include "entity/components/sprite_animator_component.h"
#include "entity/components/local_player_component.h"

using namespace bf;

void ClientContent::createPlayer(entt::entity player, WorldScene &scene, const PlayerSpawnProperties &spawnProperties) {
	World &world = scene.world;
	WorldRenderer &worldRenderer = scene.worldRenderer;
	entt::registry &entityRegistry = world.entities.registry;

	world.content.createPlayer(player, world, spawnProperties);
	
	// Load sprite
	entityRegistry.emplace<SpriteComponent>(
		player, SpriteComponent { glm::vec2(1.0f), glm::vec2(-4.0f, -2.0f) / 16.0f });
	
	entityRegistry.emplace<SpriteAnimatorComponent>(
		player, SpriteAnimatorComponent { &playerAnimations });
}

void ClientContent::createLocalPlayer(entt::entity player, WorldScene &scene, const PlayerSpawnProperties &spawnProperties) {
	World &world = scene.world;
	entt::registry &entityRegistry = world.entities.registry;

	// Spawn remote player with extra attributes
	createPlayer(player, scene, spawnProperties);

	// Start last position as current
	entityRegistry.emplace<LocalPlayerComponent>(
		player, LocalPlayerComponent { spawnProperties.position });
	
	entityRegistry.emplace<BodyComponent>(player, BodyComponent { glm::vec2(8.0f, 13.0f) / 16.0f });
}

ClientContent::ClientContent(WorldScene &scene) {
	World &world = scene.world;
	WorldRenderer &worldRenderer = scene.worldRenderer;

	// Add entity systems in order
	world.entities.addSystem(localPlayerSystem);
	world.entities.addSystem(spriteAnimatorSystem);

	// Load texture atlas
    std::vector<std::string> texturePaths = {
		"assets/textures/player.png",
		"assets/textures/tile.png",
	};

	worldRenderer.textureAtlas.loadAtlas(texturePaths);

	// Create player animations
    TextureSection playerTextureSection = scene.worldRenderer.textureAtlas.getSection("assets/textures/player.png");
    playerAnimations.frames.loadFrames(playerTextureSection.uvBox, { 8, 1 });

	playerAnimations.addAnimation({ 0 });
	playerAnimations.addAnimation({ 1, 2, 3, 4 }, 0.4f);
	playerAnimations.addAnimation({ 5 });
	playerAnimations.addAnimation({ 6 });
	playerAnimations.addAnimation({ 7 });

	localPlayerSystem.loadContent(scene);

	// Create test block renderer
    testBlockRenderer.sprite.box.size = glm::vec2(1.0f);
    testBlockRenderer.sprite.uvBox = worldRenderer.textureAtlas.getSection("assets/textures/tile.png").uvBox;

    world.blocks.registry.emplace<BlockRendererComponent>(world.content.testBlock, BlockRendererComponent { &testBlockRenderer });

	// TODO: Recieve local player ID
	player = world.entities.spawnEntity(-1);

	PlayerSpawnProperties spawnProperties;
	createLocalPlayer(player, scene, spawnProperties);
}