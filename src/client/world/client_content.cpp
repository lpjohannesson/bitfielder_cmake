#include "client_content.h"
#include "client/scenes/world_scene.h"
#include "entity/components/sprite_component.h"
#include "entity/components/sprite_animator_component.h"
#include "entity/components/sprite_aim_component.h"
#include "entity/components/local_player_component.h"

using namespace bf;

void ClientContent::createPlayer(entt::entity player, WorldScene &scene) {
	World &world = scene.world;
	WorldRenderer &worldRenderer = scene.worldRenderer;
	entt::registry &entityRegistry = world.entities.registry;

	world.content.createPlayer(player, world);
	
	// Load sprite
	entityRegistry.emplace<SpriteComponent>(
		player, SpriteComponent { glm::vec2(1.0f), glm::vec2(-4.0f, -2.0f) / 16.0f });

	SpriteAimComponent &spriteAim = entityRegistry.emplace<SpriteAimComponent>(
		player, SpriteAimComponent { &playerForwardFrames, &playerUpFrames, &playerDownFrames });
	
	entityRegistry.emplace<SpriteAnimatorComponent>(
		player, SpriteAnimatorComponent { &playerForwardFrames, &playerAnimations });
}

void ClientContent::createLocalPlayer(entt::entity player, WorldScene &scene) {
	World &world = scene.world;
	entt::registry &entityRegistry = world.entities.registry;

	// Spawn remote player with extra attributes
	createPlayer(player, scene);

	entityRegistry.emplace<LocalPlayerComponent>(
		player, LocalPlayerComponent {});
	
	entityRegistry.emplace<VelocityComponent>(player, VelocityComponent {});
	entityRegistry.emplace<BodyComponent>(player, BodyComponent { glm::vec2(8.0f, 13.0f) / 16.0f });
}

ClientContent::ClientContent(WorldScene &scene) {
	World &world = scene.world;
	WorldRenderer &worldRenderer = scene.worldRenderer;
	TextureAtlas &textureAtlas = worldRenderer.textureAtlas;

	// Add entity systems in order
	world.entities.addSystem(localPlayerSystem);
	localPlayerSystem.loadScene(scene);

	world.entities.addSystem(particleSystem);
	particleSystem.loadScene(scene);

	world.entities.addSystem(spriteAnimatorSystem);
	world.entities.addSystem(effectSpriteSystem);

	// Create player animations
    TextureSection playerForwardTexture = textureAtlas.getSection("assets/textures/world/player/forward.png");
	TextureSection playerUpTexture = textureAtlas.getSection("assets/textures/world/player/up.png");
	TextureSection playerDownTexture = textureAtlas.getSection("assets/textures/world/player/down.png");

    playerForwardFrames.loadFrames(playerForwardTexture.uvBox, { 4, 3 });
	playerUpFrames.loadFrames(playerUpTexture.uvBox, { 4, 3 });
	playerDownFrames.loadFrames(playerDownTexture.uvBox, { 4, 3 });

	playerAnimations.addAnimation({ 0 });
	playerAnimations.addAnimation({ 4, 5, 6, 7 }, 0.4f);
	playerAnimations.addAnimation({ 1 });
	playerAnimations.addAnimation({ 2 });
	playerAnimations.addAnimation({ 3 });
	playerAnimations.addAnimation({ 8, 9, 2 }, 0.3f, false);

	// TODO: Recieve local player ID
	player = world.entities.spawnEntity(-1);
	createLocalPlayer(player, scene);

	// Create effect sprites
	placeEffectProperties.createProperties(
        { 1.0f, 1.0f },
        textureAtlas.getSection("assets/textures/world/effects/place.png").uvBox,
        { 3, 1 },
		{ 0, 1, 2 },
		0.3f);

	destroyEffectProperties.createProperties(
        { 1.0f, 1.0f },
        textureAtlas.getSection("assets/textures/world/effects/destroy.png").uvBox,
        { 3, 1 },
		{ 0, 1, 2 },
		0.3f);
	
	groundEffectProperties.createProperties(
        { 1.0f, 1.0f },
        textureAtlas.getSection("assets/textures/world/effects/ground.png").uvBox,
        { 3, 1 },
		{ 0, 1, 2 },
		0.3f);
	
	// Load sounds
	placeSound = soundSet.loadSound("assets/sounds/place.wav");
	destroySound = soundSet.loadSound("assets/sounds/destroy.wav");
	jumpSound = soundSet.loadSound("assets/sounds/jump.wav");
	groundSound = soundSet.loadSound("assets/sounds/ground.wav");

	blockSounds.loadSounds();
}