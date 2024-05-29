#include "client_content.h"
#include "client/scenes/world_scene.h"
#include "block/components/block_renderer_component.h"
#include "entity/components/sprite_component.h"
#include "entity/components/sprite_animator_component.h"
#include "entity/components/local_player_component.h"

using namespace bf;

void ClientContent::createPlayer(entt::entity player, WorldScene &scene, glm::vec2 position) {
	World &world = scene.world;
	WorldRenderer &worldRenderer = scene.worldRenderer;
	entt::registry &entityRegistry = world.entities.registry;

	world.content.createPlayer(player, world, position);
	
	// Load sprite
	entityRegistry.emplace<SpriteComponent>(
		player, SpriteComponent { glm::vec2(1.0f), glm::vec2(-4.0f, -2.0f) / 16.0f });
	
	entityRegistry.emplace<SpriteAnimatorComponent>(
		player, SpriteAnimatorComponent {});
}

void ClientContent::createLocalPlayer(entt::entity player, WorldScene &scene, glm::vec2 position) {
	World &world = scene.world;
	entt::registry &entityRegistry = world.entities.registry;

	// Spawn remote player with extra attributes
	createPlayer(player, scene, position);

	entityRegistry.emplace<LocalPlayerComponent>(
		player, LocalPlayerComponent {});
	
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
    playerFrames.loadFrames(playerTextureSection.uvBox, { 6, 1 });

	playerIdle.createAnimation(playerFrames, { 0 });
	playerWalk.createAnimation(playerFrames, { 1, 2, 3, 4 }, 0.4f);
	playerJump.createAnimation(playerFrames, { 4 });
	playerSlide.createAnimation(playerFrames, { 5 });

	localPlayerSystem.loadContent(scene);

	// Create test block renderer
    testBlockRenderer.sprite.box.size = glm::vec2(1.0f);
    testBlockRenderer.sprite.uvBox = worldRenderer.textureAtlas.getSection("assets/textures/tile.png").uvBox;

    world.blocks.registry.emplace<BlockRendererComponent>(world.content.testBlock, BlockRendererComponent { &testBlockRenderer });

	// TODO: Recieve local player ID
	player = world.entities.spawnEntity(-1);
	createLocalPlayer(player, scene, { 0.0f, 0.0f });
}