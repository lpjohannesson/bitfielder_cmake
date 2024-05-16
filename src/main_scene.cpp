#include "main_scene.h"
#include "engine.h"

using namespace bf;

void MainScene::update() {

}

void MainScene::render() {
	engine.renderer.clearScreen({ 0.0f, 0.0f, 0.5f, 0.0f });
	spriteRenderer.renderMesh(spriteMesh, textureAtlas.texture);
}

void MainScene::start() {
	spriteRenderer.start();
	spriteMesh.start(spriteRenderer);

	textureAtlas.start();

	std::vector<std::string> texturePaths = {
		"assets/textures/1.png",
		"assets/textures/2.png",
		"assets/textures/3.png",
		"assets/textures/4.png",
		"assets/textures/5.png",
		"assets/textures/6.png",
		"assets/textures/7.png",
		"assets/textures/8.png",
	};

	textureAtlas.loadAtlas(texturePaths);

	Sprite sprite;
	sprite.box = { { 0.0f, 0.0f }, { 1.0f, -1.0f } };

	sprite.uvBox = { { 0.0f, 0.0f }, { 1.0f, 1.0f } };
	//sprite.uvBox = textureAtlas.getSection("assets/textures/8.png").uvBox.section({ { 0.0f, 0.0f }, { 0.5f, 1.0f } });

	spriteBatch.drawSprite(sprite);

	spriteBatch.uploadMesh(spriteMesh);
}

void MainScene::end() {
	textureAtlas.end();

	spriteMesh.end();
	spriteRenderer.end();
}