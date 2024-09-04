#include "client_content.h"
#include "client/scenes/world_scene.h"
#include <SDL2/SDL_image.h>
#include "client/world/block/block_renderer_factory.h"
#include "client/world/item/item_renderer_factory.h"
#include "entity/components/sprite_component.h"
#include "entity/components/animator_component.h"
#include "entity/components/sprite_set_component.h"
#include "entity/components/local_player_component.h"
#include "entity/components/skin_component.h"
#include "world/entity/components/inventory_component.h"

using namespace bf;

void ClientContent::createPlayer(entt::entity player, WorldScene &scene) {
	World &world = scene.world;
	entt::registry &entityRegistry = world.entities.registry;

	world.content.entities.createPlayer(player, world);
	
	entityRegistry.emplace<AnimatorComponent>(
		player, AnimatorComponent { &playerAnimations });
	
	entityRegistry.emplace<SkinComponent>(
		player, SkinComponent { &skinTexture });
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
	entityRegistry.emplace<InventoryComponent>(player, InventoryComponent {});
}

ClientContent::ClientContent(WorldScene &scene) {
	World &world = scene.world;
	WorldRenderer &worldRenderer = scene.worldRenderer;
	TextureAtlas &textureAtlas = worldRenderer.textureAtlas;

	// Add entity systems
	scene.entitySystems.push_back(&animatorSystem);
	scene.entitySystems.push_back(&particleSystem);
	scene.entitySystems.push_back(&effectSpriteSystem);
	scene.entitySystems.push_back(&localPlayerSystem);

	// Create player animations
	glm::ivec2 playerFrameCounts = { 4, 3 };

	playerSkinSprites.loadSprites("assets/textures/world/player/skin", playerFrameCounts, textureAtlas);
	playerOverlaySprites.loadSprites("assets/textures/world/player/overlay", playerFrameCounts, textureAtlas);

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
		textureAtlas.getSection("assets/textures/world/effects/place.png").uvBox,
        { 3, 1 },
		{ 0, 1, 2 },
		0.3f,
		{ 1.0f, 1.0f });

	destroyEffectProperties.createProperties(
		textureAtlas.getSection("assets/textures/world/effects/destroy.png").uvBox,
        { 3, 1 },
		{ 0, 1, 2 },
		0.3f,
		{ 1.0f, 1.0f });
	
	groundEffectProperties.createProperties(
		textureAtlas.getSection("assets/textures/world/effects/ground.png").uvBox,
        { 3, 1 },
		{ 0, 1, 2 },
		0.3f,
		{ 1.0f, 1.0f });
	
	// Load sounds
	placeSound = soundSet.createSound("assets/sounds/place.wav");
	destroySound = soundSet.createSound("assets/sounds/destroy.wav");
	jumpSound = soundSet.createSound("assets/sounds/jump.wav");
	groundSound = soundSet.createSound("assets/sounds/ground.wav");
	selectItemSound = soundSet.createSound("assets/sounds/select_item.wav");

	jumpSound.volume = 0.75f;

	blockSounds.loadSounds();

	BlockRendererFactory::start(scene);
	ItemRendererFactory::start(scene);

	// Load skin
	SDL_Surface *skinSurface = IMG_Load("assets/textures/skins/skin.png");
	skinTexture.loadSurface(skinSurface);
	SDL_FreeSurface(skinSurface);

	// Set up inventory
	ItemContent &items = world.content.items;

	std::vector<entt::entity> itemsList = {
		items.dirt,
		items.stone,
		items.woodLog,
		items.woodPlanks,
		items.stick,
		items.leaves,
		items.bush,
		items.mushroom,
		items.torch,
		items.iron
	};

	Inventory &inventory = world.entities.registry.get<InventoryComponent>(player).inventory;

	for (int i = 0; i < (int)itemsList.size(); i++) {
		inventory.setItem(i, itemsList[i], world);
	}
}