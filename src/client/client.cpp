#include "client.h"
#include <SDL2/SDL_image.h>
#include "core/file_loader.h"
#include "font_factory.h"
#include "scenes/menu_scene.h"
#include "gfx/core/shader.h"

using namespace bf;

Client *bf::client;

void Client::renderLogo(TextureSection texture, SpriteMesh &mesh) {
    SpriteBatch spriteBatch;
    Sprite &logoSprite = spriteBatch.createSprite();

    logoSprite.uvBox = texture.uvBox;
    logoSprite.box.size = glm::vec2(texture.box.size) * 2.0f;

    // Center bottom
    logoSprite.box.start.x = -logoSprite.box.size.x * 0.5f;
    logoSprite.box.start.y = -logoSprite.box.size.y;

    spriteBatch.uploadMesh(mesh);
}

float Client::getRandomPitch() {
    return 1.0f - maxRandomPitch + random.randomFloat(random.randomEngine) * maxRandomPitch * 2;
}

void Client::playRandomPitchSound(Sound &sound) {
    sound.pitch = getRandomPitch();
    engine->sound.playSound(sound);
}

void Client::updateSize(glm::ivec2 size) {
    menuTransform = glm::scale(engine->getWindowTransform(), glm::vec3(3.0f));
    menuInverseTransform = glm::inverse(menuTransform);
}

Client::Client(Engine &engine) {
    client = this;

    // Create shaders
    GLuint
        glVertexShader = glCreateShader(GL_VERTEX_SHADER),
        glCommonShader = glCreateShader(GL_FRAGMENT_SHADER),
        glFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	Shader::compileShader(glVertexShader, "assets/shaders/vertex.glsl");
    Shader::compileShader(glCommonShader, "assets/shaders/common.glsl");
    Shader::compileShader(glFragmentShader, "assets/shaders/fragment.glsl");

    spriteProgram.attachShader(glVertexShader);
    spriteProgram.attachShader(glCommonShader);
    spriteProgram.attachShader(glFragmentShader);
    spriteProgram.link();

    spriteProgram.assignTexture(1, "fTexture");

	glDeleteShader(glVertexShader);
    glDeleteShader(glCommonShader);
	glDeleteShader(glFragmentShader);

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