#include "local_world_scene.h"

using namespace bf;

void LocalWorldScene::start() {
    WorldScene::start();

    // Connect to server
    server.addClient(&localClientConnection);
}