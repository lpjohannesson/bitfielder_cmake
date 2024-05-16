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

	Sprite sprite1;
	sprite1.box = { { 0.0f, 0.0f }, { 1.0f, -1.0f } };
	sprite1.uvBox = { { 0.0f, 0.0f }, { 1.0f, 1.0f } };

	Sprite sprite2;
	sprite2.box = { { 0.0f, 1.0f }, { 1.0f, -1.0f } };
	sprite2.uvBox = textureAtlas.getSection("assets/textures/8.png").uvBox.section({ { 0.0f, 0.0f }, { 0.5f, 1.0f } });

	Sprite sprite3;
	sprite3.box = { { -1.0f, 1.0f }, { 1.0f, -1.0f } };
	sprite3.uvBox = textureAtlas.getSection("assets/textures/1.png").uvBox;

	spriteBatch.drawSprite(sprite1);
	spriteBatch.drawSprite(sprite2);
	spriteBatch.drawSprite(sprite3);

	spriteBatch.uploadMesh(spriteMesh);
}

void MainScene::end() {
	textureAtlas.end();

	spriteMesh.end();
	spriteRenderer.end();
}