#include "local_server_connection.h"
#include "scenes/local_world_scene.h"

using namespace bf;

void LocalServerConnection::writePacket(Packet &packet) {
    // Pass packet to world scene
    worldScene->localServer.readPacket(&worldScene->localClientConnection, packet);
}

LocalServerConnection::LocalServerConnection(LocalWorldScene *worldScene) {
    this->worldScene = worldScene;
}