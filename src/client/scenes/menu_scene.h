#pragma once
#include "engine/scene.h"
#include "gfx/core/texture_atlas.h"
#include "gfx/core/texture.h"
#include "client/option_list.h"
#include "client/client.h"

namespace bf {
	class MenuScene : public Scene {
    public:
        TextureAtlas textureAtlas;
        Texture texture;

        SpriteMesh logoMesh;

        OptionList optionList;

        void update() override;
        void render() override;

        void start() override;

        inline MenuScene() : logoMesh(client->spriteRenderer) {}
	};
}