#include "local_client_connection.h"
#include "scenes/local_world_scene.h"

using namespace bf;

void LocalClientConnection::writePacket(Packet &packet) {
    // Pass packet to world scene
    scene->readPacket(packet);
}

LocalClientConnection::LocalClientConnection(LocalWorldScene *scene) {
    this->scene = scene;
}