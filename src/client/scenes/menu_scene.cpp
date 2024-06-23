#include "menu_scene.h"
#include "engine/engine.h"
#include "core/file_loader.h"
#include "local_world_scene.h"
#include "remote_world_scene.h"

using namespace bf;

void MenuScene::updateSize(glm::ivec2 size) {
    menuTransform = Client::getMenuTransform();
}

void MenuScene::update() {
    switch (menuState) {
        case MenuState::HOME: {
            homeOptionList.update();
            ListOption *pressedOption = homeOptionList.getPressedOption();

            if (pressedOption == nullptr) {
                break;
            }

            if (pressedOption == &localPlayOption) {
                engine->changeScene(new LocalWorldScene());
                break;
            }

            if (pressedOption == &remotePlayOption) {
                menuState = MenuState::REMOTE;
                break;
            }

            if (pressedOption == &quitOption) {
                engine->quitting = true;
                break;
            }

            break;
        }
        
        case MenuState::REMOTE: {
            remoteOptionList.update();
            ListOption *pressedOption = remoteOptionList.getPressedOption();

            if (pressedOption == nullptr) {
                break;
            }

            if (pressedOption == &remoteBackOption) {
                menuState = MenuState::HOME;
                break;
            }
            
            break;
        }

    }
}

void MenuScene::render() {
    engine->renderer.clearScreen({ 0.0f, 0.5f, 0.5f, 1.0f });

    client->spriteProgram.setTransform(menuTransform);

    // Render logo
    client->spriteRenderer.renderMesh(logoMesh, client->spriteProgram, texture);

    // Render menu
    switch (menuState) {
        case MenuState::HOME:
            homeOptionList.render();
            break;
        
        case MenuState::REMOTE:
            remoteOptionList.render();
            break;
    }
}

void MenuScene::start() {
    // Load texture atlas
    std::vector<std::string> texturePaths;
	FileLoader::getFilePaths("assets/textures/menu", texturePaths);

	SDL_Surface *surface = textureAtlas.loadSurface(texturePaths);

	texture.loadSurface(surface);
	SDL_FreeSurface(surface);

    localPlayOption.text = "Local play";
    remotePlayOption.text = "Remote play";
    quitOption.text = "Quit game";

    homeOptionList.setOptions({ &localPlayOption, &remotePlayOption, &quitOption });
    
    remoteIPOption.headerText = "Enter IP:";

    remoteConnectOption.text = "Connect";
    remoteBackOption.text = "Back";

    remoteOptionList.setOptions({ &remoteIPOption, &remoteConnectOption, &remoteBackOption });

    // Draw logo
    TextureSection logoTexture = textureAtlas.getSection("assets/textures/menu/logo.png");
    Client::renderLogo(logoTexture, logoMesh);
}