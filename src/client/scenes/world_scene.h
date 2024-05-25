#pragma once
#include "engine/scene.h"
#include "world/world.h"
#include "client/world/client_content.h"
#include "core/packet.h"
#include "server/server.h"
#include "client/local_client_connection.h"

namespace bf {
	class WorldScene : public Scene {
	public:
		Server server;
		LocalClientConnection localClientConnection;

		World world;

		ClientContent clientContent;

		void readPacket(Packet &packet);

		void updateSize(glm::ivec2 size) override;

		void update() override;
		void render() override;

		void start() override;
		void end() override;

		inline WorldScene() : localClientConnection(this) {}
	};
}