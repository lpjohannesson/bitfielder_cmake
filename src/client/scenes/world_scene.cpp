#include "world_scene.h"
#include "engine/engine.h"
#include <iostream>

using namespace bf;

void WorldScene::readPacket(Packet &packet) {
	int i1, i2;
	packet << i1 << i2;

	std::cout << i1 << std::endl << i2 << std::endl;
}

void WorldScene::updateSize(glm::ivec2 size) {
	world.renderer.updateSize(size);
}

void WorldScene::update() {
	world.update();
}

void WorldScene::render() {
	engine->renderer.clearScreen({ 0.0f, 0.0f, 0.5f, 0.0f });
	world.renderer.render(world);
}

void WorldScene::start() {
	// Connect to server
	server.addClient(&localClientConnection);

	clientContent.loadContent(world);

	BlockChunk &chunk = world.map.createChunk(0);
	BlockChunk &chunk2 = world.map.createChunk(2);

	world.renderer.map.createMesh(world, chunk);
	world.renderer.map.createMesh(world, chunk2);
}

void WorldScene::end() {

}