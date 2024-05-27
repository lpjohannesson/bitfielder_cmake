#include "world_scene.h"
#include <cstring>
#include <iostream>
#include "engine/engine.h"
#include "core/packet_types.h"

using namespace bf;

void WorldScene::readBlockChunk(Packet &packet) {
	int chunkIndex;
	packet >> chunkIndex;

	// Chunk data
	char* chunkData;
	packet.read(chunkData, sizeof(BlockChunk::data));

	BlockChunk &chunk = world.map.createChunk(chunkIndex);
	std::memcpy(chunk.data, chunkData, sizeof(BlockChunk::data));

	// Create mesh
	worldRenderer.map.createMesh(world, chunk);
}

void WorldScene::readDespawnEntity(Packet &packet) {
	int entityID;
	packet >> entityID;

	world.entities.despawnEntity(entityID);
}

void WorldScene::readEntityPosition(Packet &packet) {
	int entityID;
	glm::vec2 newPosition;

	packet >> entityID >> newPosition;

	// Get entity
	entt::entity entity;
	
	if (!world.entities.getEntity(entityID, entity)) {
		return;
	}

	// Write value
	entt::registry &entityRegistry = world.entities.registry;

	PositionComponent &position = entityRegistry.get<PositionComponent>(entity);
	position.position = newPosition;
}

void WorldScene::readRemotePlayer(Packet &packet) {
	int playerID;
	glm::vec2 playerPosition;
	packet >> playerID >> playerPosition;

	// Spawn player entity
	entt::entity player = world.entities.spawnEntity(playerID);
	clientContent.createPlayer(player, *this, playerPosition);
}

void WorldScene::writePlayerPosition() {
	Packet packet;

	entt::registry &entityRegistry = world.entities.registry;
	glm::vec2 playerPosition = entityRegistry.get<PositionComponent>(clientContent.player).position;

	packet << playerPosition;
	server->writePacket(packet);
}

void WorldScene::readPacket(Packet &packet) {
	// Packet ID
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
	writePlayerPosition();
}

void WorldScene::render() {
	engine->renderer.clearScreen({ 0.0f, 0.0f, 0.5f, 0.0f });
	worldRenderer.render(*this);
}

void WorldScene::start() {
	clientContent.loadContent(*this);
}

void WorldScene::end() {

}