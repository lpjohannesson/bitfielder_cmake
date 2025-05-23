#include "menu_scene.h"
#include "engine/engine.h"
#include "core/file_loader.h"
#include "core/game_time.h"
#include "gfx/core/renderer.h"
#include "client/local_server_connection.h"

using namespace bf;

void MenuScene::endRemoteServerConnection() {
    remoteServerConnection->endConnection();

    delete remoteServerConnection;
    remoteServerConnection = nullptr;
}

void MenuScene::changeState(MenuState menuState) {
    this->menuState = menuState;

    switch (menuState) {
    case MenuState::HOME: {
        homeOptionList.updateMesh();

        break;

    case MenuState::REMOTE: {
        remoteOptionList.updateMesh();

        break;
    }

    case MenuState::CONNECTING: {
        // Connect to server
        remoteServerConnection = new RemoteServerConnection();

        std::string error;

        if (remoteServerConnection->startConnection(remoteIPOption.inputText.c_str(), 1234, error)) {
            remoteTime = 0.0f;
            connectingOptionList.headerText = "Connecting to server...";
        }
        else {
            endRemoteServerConnection();
            connectingOptionList.headerText = error;
        }
        
        connectingOptionList.updateMesh();

        break;
    }
    
    case MenuState::DISCONNECTED: {
        disconnectedOptionList.updateMesh();

        break;
    }
    }
}
}

void MenuScene::updateSize(glm::ivec2 size) {
    client->updateSize(size);

    // Draw notice
    glm::vec2 noticePosition = client->getMenuInverseTransform() * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    float lineHeight = client->font.lineHeight;

    SpriteBatch noticeSpriteBatch;

    noticeFontProperties.position = { noticePosition.x, noticePosition.y - lineHeight * 2 };
    client->font.drawText("version 0.1.0.", noticeFontProperties, noticeSpriteBatch);

    noticeFontProperties.position = { noticePosition.x, noticePosition.y - lineHeight };
    client->font.drawText("copyright 2024 crossfrog.", noticeFontProperties, noticeSpriteBatch);

    noticeSpriteBatch.uploadMesh(noticeMesh);
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
            // Start local world
            WorldScene *worldScene = new WorldScene();
            LocalServerConnection *localServerConnection = new LocalServerConnection();

            localServerConnection->startScene(*worldScene);
            
            engine->changeScene(worldScene);
            break;
        }

        if (pressedOption == &remotePlayOption) {
            changeState(MenuState::REMOTE);
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

        if (pressedOption == &remoteConnectOption) {
            changeState(MenuState::CONNECTING);
            break;
        }

        if (pressedOption == &remoteBackOption) {
            changeState(MenuState::HOME);
            break;
        }
        
        break;
    }

    case MenuState::CONNECTING: {
        // Update server connection
        if (remoteServerConnection != nullptr) {
            if (remoteServerConnection->updateConnection()) {
                // Start remote world
                WorldScene *worldScene = new WorldScene();
                worldScene->server = remoteServerConnection;

                engine->changeScene(worldScene);
                return;
            }

            // Check for connection timeout
            remoteTime += gameTime.getDeltaTime();

            if (remoteTime >= maxRemoteTime) {
                endRemoteServerConnection();

                connectingOptionList.headerText = "Connection failed.";
                connectingOptionList.updateMesh();
            }
        }

        connectingOptionList.update();
        ListOption *pressedOption = connectingOptionList.getPressedOption();

        if (pressedOption == nullptr) {
            break;
        }

        if (pressedOption == &connectingBackOption) {
            if (remoteServerConnection != nullptr) {
                endRemoteServerConnection();
            }

            changeState(MenuState::REMOTE);
            
            break;
        }
    }
    
    case MenuState::DISCONNECTED: {
        disconnectedOptionList.update();
        ListOption *pressedOption = disconnectedOptionList.getPressedOption();

        if (pressedOption == nullptr) {
            break;
        }

        if (pressedOption == &disconnectedBackOption) {
            changeState(MenuState::HOME);
            
            break;
        }
    }
    }
}

void MenuScene::render() {
    Renderer::clearScreen({ 0.0f, 0.5f, 0.5f, 1.0f });

    client->spriteProgram.setTransform(client->getMenuTransform());

    // Render logo and notice
    client->spriteRenderer.renderMesh(logoMesh, client->spriteProgram, texture);
    client->spriteRenderer.renderMesh(noticeMesh, client->spriteProgram, client->fontTexture);

    // Render menu
    switch (menuState) {
        case MenuState::HOME:
            homeOptionList.render();
            break;
        
        case MenuState::REMOTE:
            remoteOptionList.render();
            break;

        case MenuState::CONNECTING:
            connectingOptionList.render();
            break;
        
        case MenuState::DISCONNECTED:
            disconnectedOptionList.render();
            break;
    }
}

MenuScene::MenuScene() :
    logoMesh(client->spriteRenderer),
    noticeMesh(client->spriteRenderer),
    homeOptionList(optionListRenderer),
    remoteOptionList(optionListRenderer),
    connectingOptionList(optionListRenderer),
    disconnectedOptionList(optionListRenderer) {
    
    maxRemoteTime = 5.0f;

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

    connectingBackOption.text = "Back";

    connectingOptionList.setOptions({ &connectingBackOption });

    disconnectedBackOption.text = "Back";

    disconnectedOptionList.headerText = "Disconnected from server.";
    disconnectedOptionList.setOptions({ &disconnectedBackOption });

    // Draw logo
    TextureSection logoTexture = textureAtlas.getSection("assets/textures/menu/logo.png");
    Client::renderLogo(logoTexture, logoMesh);

    noticeFontProperties.centered = true;
}