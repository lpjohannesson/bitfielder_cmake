#include "main_scene.h"
#include "engine/engine.h"
#include <iostream>

using namespace bf;

void MainScene::updateSize(glm::ivec2 size) {
	world.renderer.updateSize(size);
}

void MainScene::update() {
	world.update();
}

void MainScene::render() {
	engine->renderer.clearScreen({ 0.0f, 0.0f, 0.5f, 0.0f });
	world.renderer.render(world);
}

void MainScene::start() {
	clientContent.loadContent(world);

	BlockChunk &chunk = world.map.createChunk(0);
	BlockChunk &chunk2 = world.map.createChunk(1);

	world.renderer.map.createMesh(world, chunk);
	world.renderer.map.createMesh(world, chunk2);
}

void MainScene::end() {

}