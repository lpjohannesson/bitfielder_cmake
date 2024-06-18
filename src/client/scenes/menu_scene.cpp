#include "menu_scene.h"
#include "engine/engine.h"
#include "core/file_loader.h"

using namespace bf;

void MenuScene::update() {
    optionList.update();
}

void MenuScene::render() {
    engine->renderer.clearScreen({ 0.0f, 0.5f, 0.5f, 1.0f });

    client->spriteProgram.setTransform(glm::scale(engine->getWindowTransform(), glm::vec3(3.0f)));

    // Render logo
    client->spriteRenderer.renderMesh(logoMesh, client->spriteProgram, texture);

    optionList.render();
}

void MenuScene::start() {
    // Load texture atlas
    std::vector<std::string> texturePaths;
	FileLoader::getFilePaths("assets/textures/menu", texturePaths);

	SDL_Surface *surface = textureAtlas.loadSurface(texturePaths);

	texture.loadSurface(surface);
	SDL_FreeSurface(surface);

    optionList.setOptions({ "Play locally", "Join a server", "Quit game" });

    // Draw logo
    TextureSection logoTexture = textureAtlas.getSection("assets/textures/menu/logo.png");

    SpriteBatch logoSpriteBatch;
    Sprite logoSprite;

    logoSprite.uvBox = logoTexture.uvBox;
    logoSprite.box.size = glm::vec2(logoTexture.box.size) * 2.0f;

    logoSprite.box.start.x = -logoSprite.box.size.x * 0.5f;
    logoSprite.box.start.y = -logoSprite.box.size.y;

    logoSpriteBatch.drawSprite(logoSprite);
    logoSpriteBatch.uploadMesh(logoMesh);
}