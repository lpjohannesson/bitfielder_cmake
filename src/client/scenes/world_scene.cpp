#include "world_scene.h"
#include <cstring>
#include <iostream>
#include "engine/engine.h"
#include "core/packet_types.h"
#include "core/game_time.h"
#include "client/world/entity/components/local_player_component.h"
#include "world/entity/components/sprite_flip_component.h"

using namespace bf;

void WorldScene::updateBlock(glm::ivec2 position) {
	// TODO: Block particles

	// Get sample including neighbours
	BlockSample<BlockChunk> blockSample(world.map, position.x - 1, position.x + 1);

	// Update meshes
	for (BlockChunk *chunk : blockSample.chunks) {
		if (chunk == nullptr) {
			continue;
		}

		worldRenderer.map.createMesh(world, *chunk);
	}
}

void WorldScene::writePlayerPosition() {
	Packet packet;

	glm::vec2 position = entityRegistry->get<PositionComponent>(clientContent.player).position;

	packet << (int)ClientPacket::PLAYER_POSITION << position;
	server->writePacket(packet);
}

void WorldScene::writePlayerSpriteAnimation() {
	Packet packet;

	SpriteAnimationComponent &animation = entityRegistry->get<SpriteAnimationComponent>(clientContent.player);

	packet << (int)ClientPacket::PLAYER_SPRITE_ANIMATION << animation.animationIndex;
	server->writePacket(packet);
}

void WorldScene::writePlayerSpriteFlip() {
	Packet packet;

	SpriteFlipComponent &spriteFlip = entityRegistry->get<SpriteFlipComponent>(clientContent.player);

	packet << (int)ClientPacket::PLAYER_SPRITE_FLIP << spriteFlip.flipX;
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
	worldRenderer.map.createMesh(world, chunk);

	// Update neighbour meshes
	BlockChunk
		*leftChunk = world.map.getChunk(chunkIndex - 1),
		*rightChunk = world.map.getChunk(chunkIndex + 1);
	
	if (leftChunk != nullptr) {
		worldRenderer.map.createMesh(world, *leftChunk);
	}

	if (rightChunk != nullptr) {
		worldRenderer.map.createMesh(world, *rightChunk);
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

void WorldScene::readRemotePlayer(Packet &packet) {
	int playerID;

	packet >> playerID;
	
	PlayerSpawnProperties spawnProperties;
	packet >> spawnProperties.position >> spawnProperties.spriteAnimationIndex >> spawnProperties.spriteFlipX;

	// Spawn player entity
	entt::entity player = world.entities.spawnEntity(playerID);
	clientContent.createPlayer(player, *this, spawnProperties);
}

void WorldScene::readPacket(Packet &packet) {
	int packetID;
	packet >> packetID;

	switch ((ServerPacket)packetID) {
	case ServerPacket::BLOCK_CHUNK:
		readBlockChunk(packet);
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
	
	case ServerPacket::REMOTE_PLAYER:
		readRemotePlayer(packet);
		break;
	}
}

void WorldScene::updateSize(glm::ivec2 size) {
	worldRenderer.updateSize(size);
}

void WorldScene::update() {
	world.update();
	camera.update(*this);

	LocalPlayerComponent &localPlayer = entityRegistry->get<LocalPlayerComponent>(clientContent.player);

	if (localPlayer.positionDirty) {
		writePlayerPosition();
	}

	if (localPlayer.spriteAnimationDirty) {
		writePlayerSpriteAnimation();
	}

	if (localPlayer.spriteFlipDirty) {
		writePlayerSpriteFlip();
	}
}

void WorldScene::render() {
	engine->renderer.clearScreen({ 0.5f, 1.0f, 1.0f, 0.0f });
	worldRenderer.render(*this);
}

void WorldScene::start() {
	gameTime.reset();
	camera.start(*this);
}

void WorldScene::end() {

}

WorldScene::WorldScene() : worldRenderer(*this), clientContent(*this) {
	entityRegistry = &world.entities.registry;
}