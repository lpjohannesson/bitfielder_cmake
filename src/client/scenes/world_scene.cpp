#include "world_scene.h"
#include <cstring>
#include "engine/engine.h"
#include <iostream>

using namespace bf;

void WorldScene::readChunk(Packet &packet) {
	// Chunk index
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
	clientContent.spawnRemotePlayer(*this, { 0.0f, 0.0f });
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
	}
}

void WorldScene::updateSize(glm::ivec2 size) {
	worldRenderer.updateSize(size);
}

void WorldScene::update() {
	world.update();

	Packet packet;
	packet << 12345;
	server->writePacket(packet);
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