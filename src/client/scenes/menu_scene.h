#pragma once
#include "engine/scene.h"
#include "gfx/sprite/sprite_mesh.h"
#include "client/client.h"

namespace bf {
	class MenuScene : public Scene {
    public:
        SpriteBatch textSpriteBatch;
        SpriteMesh textMesh;

        void render() override;

        void start() override;

        inline MenuScene() : textMesh(client->spriteRenderer) {}
	};
}