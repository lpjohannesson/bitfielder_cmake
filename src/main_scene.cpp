#include "main_scene.h"
#include "engine/engine.h"

using namespace bf;

void MainScene::updateSize(glm::ivec2 size) {
	worldRenderer.updateSize(size);
}

void MainScene::update() {

}

void MainScene::render() {
	engine->renderer.clearScreen({ 0.0f, 0.0f, 0.5f, 0.0f });
	worldRenderer.render(world);
}

void MainScene::start() {
	clientContent.loadContent(world, worldRenderer);

	BlockChunk &chunk = world.map.createChunk(0);
	worldRenderer.map.createMesh(worldRenderer, world, chunk);
}

void MainScene::end() {

}