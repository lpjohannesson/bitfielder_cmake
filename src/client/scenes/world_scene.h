#pragma once
#include "engine/scene.h"
#include "world/world.h"
#include "client/server_connection.h"
#include "client/world/world_renderer.h"
#include "client/world/client_content.h"

namespace bf {
	class WorldScene : public Scene {
	public:
		ServerConnection *server;

		World world;
		WorldRenderer worldRenderer;

		ClientContent clientContent;

		void readChunk(Packet &packet);
		void readRemotePlayer(Packet &packet);

		void readPacket(Packet &packet);

		void updateSize(glm::ivec2 size) override;

		void update() override;
		void render() override;

		void start() override;
		void end() override;
	};
}