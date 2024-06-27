#pragma once
#include "engine/scene.h"
#include "gfx/core/texture_atlas.h"
#include "gfx/core/texture.h"
#include "client/menu/option_list.h"
#include "client/menu/option_list_renderer.h"
#include "client/client.h"
#include "client/menu/text_list_option.h"
#include "client/menu/input_list_option.h"
#include "client/remote_server_connection.h"

namespace bf {
    enum class MenuState {
        HOME, REMOTE, CONNECTING, DISCONNECTED
    };

	class MenuScene : public Scene {
    public:
        TextureAtlas textureAtlas;
        Texture texture;

        SpriteMesh logoMesh;

        TextListOption
            localPlayOption,
            remotePlayOption,
            quitOption,

            remoteConnectOption,
            remoteBackOption,

            connectingBackOption,
            
            disconnectedBackOption;
        
        InputListOption remoteIPOption;

        OptionList homeOptionList, remoteOptionList, connectingOptionList, disconnectedOptionList;
        OptionListRenderer optionListRenderer;

        MenuState menuState;

        glm::mat4 menuTransform;

        RemoteServerConnection *remoteServerConnection = nullptr;
        float remoteTime = 0.0f, maxRemoteTime;

        void endRemoteServerConnection();

        void changeState(MenuState menuState);

        void updateSize(glm::ivec2 size) override;

        void update() override;
        void render() override;

        MenuScene();
	};
}