#include "client_content.h"
#include <filesystem>
#include "client/scenes/world_scene.h"
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
	
	entityRegistry.emplace<VelocityComponent>(player, VelocityComponent {});
	entityRegistry.emplace<BodyComponent>(player, BodyComponent { glm::vec2(8.0f, 13.0f) / 16.0f });
}

ClientContent::ClientContent(WorldScene &scene) {
	World &world = scene.world;
	WorldRenderer &worldRenderer = scene.worldRenderer;

	// Add entity systems in order
	world.entities.addSystem(localPlayerSystem);
	localPlayerSystem.loadScene(scene);

	world.entities.addSystem(particleSystem);
	particleSystem.loadScene(scene);

	world.entities.addSystem(spriteAnimatorSystem);

	// Load texture atlas
    std::vector<std::string> texturePaths;

	for (auto &textureEntry : std::filesystem::recursive_directory_iterator("assets/textures")) {
		if (textureEntry.is_directory()) {
			continue;
		}

		texturePaths.push_back(textureEntry.path().string());
	}

	worldRenderer.textureAtlas.loadAtlas(texturePaths);

	// Create player animations
    TextureSection playerTextureSection = scene.worldRenderer.textureAtlas.getSection("assets/textures/player.png");
    playerAnimations.frames.loadFrames(playerTextureSection.uvBox, { 8, 1 });

	playerAnimations.addAnimation({ 0 });
	playerAnimations.addAnimation({ 1, 2, 3, 4 }, 0.4f);
	playerAnimations.addAnimation({ 5 });
	playerAnimations.addAnimation({ 6 });
	playerAnimations.addAnimation({ 7 });

	// TODO: Recieve local player ID
	player = world.entities.spawnEntity(-1);

	PlayerSpawnProperties spawnProperties;
	createLocalPlayer(player, scene, spawnProperties);
}