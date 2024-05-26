#include "local_client_connection.h"
#include "scenes/local_world_scene.h"

using namespace bf;

void LocalClientConnection::writePacket(Packet &packet) {
    // Pass packet to world scene
    worldScene->readPacket(packet);
}

LocalClientConnection::LocalClientConnection(LocalWorldScene *worldScene) {
    this->worldScene = worldScene;
}