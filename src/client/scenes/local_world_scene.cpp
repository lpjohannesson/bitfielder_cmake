#include "local_world_scene.h"

using namespace bf;

void LocalWorldScene::start() {
    // Connect to server
    WorldScene::start();

    server = &localServerConnection;
    localServer.addClient(&localClientConnection);

    startClient();
}

void LocalWorldScene::end() {
    // Disconnect from server
    localServer.removeClient(&localClientConnection);

    WorldScene::end();
}