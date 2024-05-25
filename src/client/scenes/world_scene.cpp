#include "world_scene.h"
#include <cstring>
#include "engine/engine.h"
#include <iostream>

using namespace bf;

void WorldScene::readPacket(Packet &packet) {
	int chunkIndex;
	packet << chunkIndex;

	char* chunkData;
	packet.read(chunkData, sizeof(BlockChunk::data));

	BlockChunk &chunk = world.map.createChunk(chunkIndex);

	std::memcpy(chunk.data, chunkData, sizeof(BlockChunk::data));

	worldRenderer.map.createMesh(world, chunk);
}

void WorldScene::updateSize(glm::ivec2 size) {
	worldRenderer.updateSize(size);
}

void WorldScene::update() {
	world.update();
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