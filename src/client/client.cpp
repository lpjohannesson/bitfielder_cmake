#include "client.h"
#include "scenes/world_scene.h"

using namespace bf;

Client *bf::client;

Client::Client(Engine &engine) {
    client = this;

    // Change scene
    engine.changeScene(new WorldScene());
}