#pragma once
#include "engine/scene.h"
#include "gfx/core/texture_atlas.h"
#include "gfx/core/texture.h"
#include "client/menu/option_list.h"
#include "client/menu/option_list_renderer.h"
#include "client/client.h"
#include "client/menu/text_list_option.h"
#include "client/menu/input_list_option.h"

namespace bf {
    enum class MenuState {
        HOME,
        REMOTE
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
            remoteBackOption;
        
        InputListOption remoteIPOption;

        OptionList homeOptionList, remoteOptionList;
        OptionListRenderer optionListRenderer;

        MenuState menuState = MenuState::HOME;

        glm::mat4 menuTransform;

        void updateSize(glm::ivec2 size) override;

        void update() override;
        void render() override;

        void start() override;

        inline MenuScene() :
            logoMesh(client->spriteRenderer),
            homeOptionList(optionListRenderer),
            remoteOptionList(optionListRenderer) {}
	};
}