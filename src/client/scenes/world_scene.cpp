#include "world_scene.h"
#include "client/server_connection.h"
#include "engine/engine.h"
#include "core/packet_types.h"
#include "core/game_time.h"
#include "sound/sound.h"
#include "menu_scene.h"
#include "client/world/block/block_renderer_factory.h"
#include "client/world/entity/components/local_player_component.h"
#include "client/world/entity/components/sprite_aim_component.h"
#include "client/world/block/components/block_particle_component.h"
#include "client/world/entity/systems/particle_system.h"
#include "client/world/entity/systems/sprite_aim_system.h"
#include "world/entity/components/sprite_flip_component.h"
#include "world/entity/components/aim_component.h"

using namespace bf;

void WorldScene::updateBlock(glm::ivec2 position) {
	world.updateBlock(position);

	// Get sample including neighbours
	BlockSample<BlockChunk> blockSample;
	blockSample.sampleBlocks(world.map, position.x - 1, position.x + 1);

	// Update meshes
	for (BlockChunk *chunk : blockSample.chunks) {
		if (chunk == nullptr) {
			continue;
		}

		// Generate section with neigbours
		int sectionStart = glm::max(0, BlockMesh::getSectionIndex(position.y - 1));
		int sectionEnd = glm::min(BlockMesh::SECTION_COUNT - 1, BlockMesh::getSectionIndex(position.y + 1));

		worldRenderer.map.createMesh(*this, *chunk, sectionStart, sectionEnd);
	}
}

void WorldScene::spawnBlockParticles(glm::vec2 position, entt::entity block) {
	entt::registry &blockRegistry = world.blocks.registry;

	if (!blockRegistry.all_of<BlockParticleComponent>(block)) {
		return;
	}

	BlockParticleComponent blockParticle = blockRegistry.get<BlockParticleComponent>(block);
	clientContent.particleSystem.spawnParticleExplosion(position + glm::vec2(0.5f), blockParticle.size, blockParticle.frames, blockParticle.color);
}

void WorldScene::placeBlock(glm::ivec2 position, bool onFrontLayer, BlockData *blockData, int blockIndex) {
	if (onFrontLayer) {
		blockData->frontIndex = blockIndex;
	}
	else {
		blockData->backIndex = blockIndex;
	}

	entt::entity block = world.blocks.getBlock(blockIndex);

	EffectSpriteSystem::spawnEffect(world, glm::vec2(position) + glm::vec2(0.5f),
		clientContent.placeEffectProperties);

	updateBlock(position);

	engine->sound.playSound(clientContent.placeSound, false, 1.0f, client->getRandomPitch());
	BlockSounds::playBlockSound(*this, block);
}

void WorldScene::destroyBlock(glm::ivec2 position, bool onFrontLayer, BlockData *blockData) {
	int blockIndex;

	if (onFrontLayer) {
		blockIndex = blockData->frontIndex;
		blockData->frontIndex = 0;
	}
	else {
		blockIndex = blockData->backIndex;
		blockData->backIndex = 0;
	}

	entt::entity block = world.blocks.getBlock(blockIndex);

	EffectSpriteSystem::spawnEffect(world, glm::vec2(position) + glm::vec2(0.5f),
		clientContent.destroyEffectProperties);

	spawnBlockParticles(position, block);

	updateBlock(position);

	engine->sound.playSound(clientContent.destroySound, false, 1.0f, client->getRandomPitch());
	BlockSounds::playBlockSound(*this, block);
}

void WorldScene::writePlayerState() {
	Packet packet;

	glm::vec2 position = entityRegistry->get<PositionComponent>(clientContent.player).position;
	SpriteAnimationComponent &animation = entityRegistry->get<SpriteAnimationComponent>(clientContent.player);
	SpriteFlipComponent &spriteFlip = entityRegistry->get<SpriteFlipComponent>(clientContent.player);
	AimComponent &aim = entityRegistry->get<AimComponent>(clientContent.player);

	packet << (int)ClientPacket::PLAYER_STATE << position << animation.animationIndex << spriteFlip.flipX << aim.aim;
	server->writePacket(packet);
}

void WorldScene::writeReplaceBlock(glm::ivec2 position, bool onFrontLayer, BlockData *blockData) {
	Packet packet;

	packet << (int)ClientPacket::REPLACE_BLOCK << position << onFrontLayer;

	if (onFrontLayer) {
		packet << (int)blockData->frontIndex;
	}
	else {
		packet << (int)blockData->backIndex;
	}

	server->writePacket(packet);
}

bool WorldScene::readEntityPacket(Packet &packet, entt::entity &entity) {
	int entityID;
	packet >> entityID;
	
	if (!world.entities.getEntity(entityID, entity)) {
		return false;
	}

	return true;
}

void WorldScene::readBlockChunk(Packet &packet) {
	int chunkIndex;
	packet >> chunkIndex;

	// Chunk data
	char* chunkData = packet.read(sizeof(BlockChunk::data));

	BlockChunk &chunk = world.map.createChunk(chunkIndex);
	std::memcpy(chunk.data, chunkData, sizeof(BlockChunk::data));

	// Create mesh
	worldRenderer.map.createMesh(*this, chunk);

	// Update neighbour meshes
	BlockChunk
		*leftChunk = world.map.getChunk(chunkIndex - 1),
		*rightChunk = world.map.getChunk(chunkIndex + 1);
	
	if (leftChunk != nullptr) {
		worldRenderer.map.createMesh(*this, *leftChunk);
	}

	if (rightChunk != nullptr) {
		worldRenderer.map.createMesh(*this, *rightChunk);
	}
}

void WorldScene::readReplaceBlock(Packet &packet) {
	glm::ivec2 position;
	bool onFrontLayer;
	int blockIndex;

	packet >> position >> onFrontLayer >> blockIndex;

	BlockData *blockData = BlockChunk::getWorldBlock(world.map, position);

	if (blockData == nullptr) {
		return;
	}

	if (blockIndex == 0) {
		destroyBlock(position, onFrontLayer, blockData);
	}
	else {
		placeBlock(position, onFrontLayer, blockData, blockIndex);
	}
}

void WorldScene::readDespawnEntity(Packet &packet) {
	int entityID;
	packet >> entityID;

	world.entities.despawnEntity(entityID);
}

void WorldScene::readEntityPosition(Packet &packet) {
	entt::entity entity;
	
	if (!readEntityPacket(packet, entity)) {
		return;
	}

	PositionComponent &position = entityRegistry->get<PositionComponent>(entity);
	packet >> position.position;
}

void WorldScene::readEntitySpriteAnimation(Packet &packet) {
	entt::entity entity;
	
	if (!readEntityPacket(packet, entity)) {
		return;
	}

	int animationIndex;
	packet >> animationIndex;

	SpriteAnimationComponent &animation = entityRegistry->get<SpriteAnimationComponent>(entity);
	SpriteAnimatorComponent &animator = entityRegistry->get<SpriteAnimatorComponent>(entity);

	SpriteAnimatorSystem::playAnimation(animator, animation, animationIndex);
}

void WorldScene::readEntitySpriteFlip(Packet &packet) {
	entt::entity entity;
	
	if (!readEntityPacket(packet, entity)) {
		return;
	}

	SpriteFlipComponent &spriteFlip = entityRegistry->get<SpriteFlipComponent>(entity);
	packet >> spriteFlip.flipX;
}

void WorldScene::readEntitySpriteAim(Packet &packet) {
	entt::entity entity;
	
	if (!readEntityPacket(packet, entity)) {
		return;
	}

	AimComponent &aim = entityRegistry->get<AimComponent>(entity);
	SpriteAimComponent &spriteAim = entityRegistry->get<SpriteAimComponent>(entity);
	SpriteAnimatorComponent &animator = entityRegistry->get<SpriteAnimatorComponent>(entity);

	packet >> aim.aim;

	animator.frames = SpriteAimSystem::getAimFrames(spriteAim, aim.aim);
}

void WorldScene::readRemotePlayer(Packet &packet) {
	int playerID;

	packet >> playerID;

	// Spawn player entity
	entt::entity player = world.entities.spawnEntity(playerID);
	clientContent.createPlayer(player, *this);
}

void WorldScene::readPacket(Packet &packet) {
	while (!packet.ended()) {
		int packetID;
		packet >> packetID;

		switch ((ServerPacket)packetID) {
		case ServerPacket::BLOCK_CHUNK:
			readBlockChunk(packet);
			break;

		case ServerPacket::REPLACE_BLOCK:
			readReplaceBlock(packet);
			break;
		
		case ServerPacket::DESPAWN_ENTITY:
			readDespawnEntity(packet);
			break;
		
		case ServerPacket::ENTITY_POSITION:
			readEntityPosition(packet);
			break;

		case ServerPacket::ENTITY_SPRITE_ANIMATION:
			readEntitySpriteAnimation(packet);
			break;

		case ServerPacket::ENTITY_SPRITE_FLIP:
			readEntitySpriteFlip(packet);
			break;

		case ServerPacket::ENTITY_SPRITE_AIM:
			readEntitySpriteAim(packet);
			break;
		
		case ServerPacket::REMOTE_PLAYER:
			readRemotePlayer(packet);
			break;
		}
	}
}

bool WorldScene::updatePauseMenu() {
	pauseOptionList.update();
	ListOption *pressedOption = pauseOptionList.getPressedOption();

	if (pressedOption == nullptr) {
		return true;
	}

	if (pressedOption == &pauseContinueOption) {
		paused = false;
		return true;
	}

	if (pressedOption == &pauseTitleOption) {
		MenuScene *menuScene = new MenuScene();
		menuScene->changeState(MenuState::HOME);

		engine->changeScene(menuScene);
		return false;
	}

	return true;
}

void WorldScene::updateSize(glm::ivec2 size) {
	worldRenderer.updateSize(size, *this);

	menuTransform = Client::getMenuTransform();
}

void WorldScene::update() {
	if (client->clientInput.pause.justPressed()) {
		paused = !paused;
	}

	// Stop in case server ended
	if (!server->host(*this)) {
		return;
	}

	if (paused) {
		// End if pause menu ended
		if (!updatePauseMenu()) {
			return;
		}
	}
	else {
		float zoomDirection = 
        (float)client->clientInput.zoomIn.justPressed() -
        (float)client->clientInput.zoomOut.justPressed();
	
		if (zoomDirection != 0.0f) {
			camera.setZoom(glm::max(1.0f, camera.getZoom() + zoomDirection));
			worldRenderer.updateTransforms(*this);
		}
	}

	camera.update(*this);
}

void WorldScene::render() {
	worldRenderer.render(*this);

	if (paused) {
		// Render pause screen
		client->spriteProgram.setTransform(menuTransform);
		client->spriteRenderer.renderMesh(pauseLogoMesh, client->spriteProgram, worldRenderer.texture);

		pauseOptionList.render();
	}
}

void WorldScene::start() {
	gameTime.reset();

	camera.start(*this);
	camera.setZoom(3.0f);

	// Create pause screen
	TextureSection pausedLogoTexture = worldRenderer.textureAtlas.getSection("assets/textures/world/paused_logo.png");
	Client::renderLogo(pausedLogoTexture, pauseLogoMesh);

	pauseContinueOption.text = "Continue";
	pauseTitleOption.text = "Return to title";

	pauseOptionList.setOptions({ &pauseContinueOption, &pauseTitleOption });
	pauseOptionList.updateMesh();
}

void WorldScene::end() {
	BlockRendererFactory::destroyRenderers(*this);

	// End server
	server->end();
	delete server;

	engine->sound.reset();
}

WorldScene::WorldScene() :
	worldRenderer(*this),
	clientContent(*this),
	pauseOptionList(optionListRenderer),
	pauseLogoMesh(client->spriteRenderer) {

	entityRegistry = &world.entities.registry;
	BlockRendererFactory::createRenderers(*this);
}