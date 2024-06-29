#include "local_server_connection.h"

using namespace bf;

void LocalServerConnection::startScene(WorldScene &scene) {
    // Connect client and server
    scene.server = this;
    clientConnection.scene = &scene;

    server.addClient(&clientConnection);
}

bool LocalServerConnection::host(WorldScene &scene) {
    if (!scene.paused) {
        server.world.update();
        scene.world.update();
    }

    return true;
}

void LocalServerConnection::writePacket(Packet &packet) {
    // Pass packet to world scene
    server.readPacket(&clientConnection, packet);
}

void LocalServerConnection::end() {
    server.removeClient(&clientConnection);
}