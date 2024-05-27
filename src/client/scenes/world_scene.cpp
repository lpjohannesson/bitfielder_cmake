#include "world_scene.h"
#include <cstring>
#include "engine/engine.h"
#include <iostream>

using namespace bf;

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

void WorldScene::readChunk(Packet &packet) {
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

void WorldScene::readRemotePlayer(Packet &packet) {
	int playerID;
	glm::vec2 playerPosition;
	packet >> playerID >> playerPosition;

	// Spawn player entity
	entt::entity player = world.entities.spawnEntity(playerID);
	clientContent.createPlayer(player, *this, playerPosition);
}

void WorldScene::readDespawnRemotePlayer(Packet &packet) {
	int playerID;
	packet >> playerID;

	world.entities.despawnEntity(playerID);
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

	switch (packetID) {
	case 0:
		readChunk(packet);
		break;

	case 1:
		readRemotePlayer(packet);
		break;
	
	case 2:
		readDespawnRemotePlayer(packet);
		break;
	
	case 3:
		readEntityPosition(packet);
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