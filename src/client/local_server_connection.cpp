#include "local_server_connection.h"
#include "scenes/local_world_scene.h"

using namespace bf;

void LocalServerConnection::writePacket(Packet &packet) {
    // Pass packet to world scene
    scene->localServer.readPacket(&scene->localClientConnection, packet);
}

LocalServerConnection::LocalServerConnection(LocalWorldScene *scene) {
    this->scene = scene;
}