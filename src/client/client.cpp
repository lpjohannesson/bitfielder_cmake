#include "client.h"
#include <SDL2/SDL_image.h>
#include "core/file_loader.h"
#include "font_factory.h"
#include "scenes/menu_scene.h"
#include "scenes/local_world_scene.h"
#include "scenes/remote_world_scene.h"

using namespace bf;

Client *bf::client;

Client::Client(Engine &engine) : spriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl") {
    client = this;

    // Load fonts
    std::vector<std::string> fontTexturePaths;
	FileLoader::getFilePaths("assets/textures/fonts", fontTexturePaths);

    SDL_Surface *fontSurface = fontTextureAtlas.loadSurface(fontTexturePaths);
    fontTexture.loadSurface(fontSurface);
    SDL_FreeSurface(fontSurface);

    FontFactory::loadFont("font", fontTextureAtlas, font);

    engine.changeScene(new LocalWorldScene());
}