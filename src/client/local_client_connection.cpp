#include "local_client_connection.h"
#include "client/scenes/world_scene.h"

using namespace bf;

void LocalClientConnection::writePacket(Packet &packet) {
    // Pass packet to world scene
    worldScene->readPacket(packet);
}

LocalClientConnection::LocalClientConnection(WorldScene *worldScene) {
    this->worldScene = worldScene;
}