#include "client.h"
#include <SDL2/SDL_image.h>
#include "core/file_loader.h"
#include "scenes/menu_scene.h"
#include "scenes/local_world_scene.h"

using namespace bf;

Client *bf::client;

Client::Client(Engine &engine) : spriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl") {
    client = this;

    std::vector<std::string> textTexturePaths;
	FileLoader::getFilePaths("assets/textures/fonts", textTexturePaths);

    SDL_Surface *textSurface = textTextureAtlas.loadSurface(textTexturePaths);

    font.loadFont(textSurface, textTextureAtlas.getSection("assets/textures/fonts/font.png"), 4);
    fontBold.loadFont(textSurface, textTextureAtlas.getSection("assets/textures/fonts/font_bold.png"), 4);

    textTexture.loadSurface(textSurface);

    IMG_SavePNG(textSurface, "textAtlas.png");

    SDL_FreeSurface(textSurface);

    engine.changeScene(new MenuScene());
}