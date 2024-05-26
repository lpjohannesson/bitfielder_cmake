#include "local_world_scene.h"

using namespace bf;

void LocalWorldScene::start() {
    WorldScene::start();

    // Connect to server
    server = &localServerConnection;
    localServer.addClient(&localClientConnection);
}

void LocalWorldScene::end() {
    // Disconnect from server
    localServer.removeClient(&localClientConnection);

    WorldScene::end();
}