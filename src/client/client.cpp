#include "client.h"
#include "scenes/main_scene.h"

using namespace bf;

Client *bf::client;

Client::Client(Engine &engine) : clientInput(engine.input) {
    client = this;

    engine.changeScene(new MainScene());
}