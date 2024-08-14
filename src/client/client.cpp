#include "client.h"
#include <SDL2/SDL_image.h>
#include "core/file_loader.h"
#include "font_factory.h"
#include "scenes/menu_scene.h"

using namespace bf;

Client *bf::client;

void Client::renderLogo(TextureSection texture, SpriteMesh &mesh) {
    SpriteBatch spriteBatch;
    Sprite logoSprite;

    logoSprite.uvBox = texture.uvBox;
    logoSprite.box.size = glm::vec2(texture.box.size) * 2.0f;

    // Center bottom
    logoSprite.box.start.x = -logoSprite.box.size.x * 0.5f;
    logoSprite.box.start.y = -logoSprite.box.size.y;

    spriteBatch.drawSprite(logoSprite);
    spriteBatch.uploadMesh(mesh);
}

float Client::getRandomPitch() {
    return 1.0f - maxRandomPitch + randomFloat(randomEngine) * maxRandomPitch * 2;
}

void Client::updateSize(glm::ivec2 size) {
    menuTransform = glm::scale(engine->getWindowTransform(), glm::vec3(3.0f));
    menuInverseTransform = glm::inverse(menuTransform);
}

Client::Client(Engine &engine) :
    randomInt(0, INT_MAX),
    randomFloat(0.0f, 1.0f),
    spriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl") {
        
    client = this;

    // Load fonts
    std::vector<std::string> fontTexturePaths;
	FileLoader::getFilePaths("assets/textures/fonts", fontTexturePaths);

    SDL_Surface *fontSurface = fontTextureAtlas.loadSurface(fontTexturePaths);
    fontTexture.loadSurface(fontSurface);
    SDL_FreeSurface(fontSurface);

    FontFactory::loadFont("font", fontTextureAtlas, font);

    MenuScene *menuScene = new MenuScene();
    menuScene->changeState(MenuState::HOME);

    engine.changeScene(menuScene);

    maxRandomPitch = 0.15f;
}