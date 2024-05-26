#include "client.h"
#include <iostream>
#include "scenes/local_world_scene.h"
#include "scenes/remote_world_scene.h"

using namespace bf;

Client *bf::client;

Client::Client(Engine &engine) {
    client = this;

    std::cout << "\"l\" for local, \"r\" for remote." << std::endl;

    std::string clientMode;
    std::cin >> clientMode;

    if (clientMode == "l") {
        engine.changeScene(new LocalWorldScene());
    }
    else if (clientMode == "r") {
        std::cout << "Enter IP:" << std::endl;

        std::string ip;
        std::cin >> ip;

        engine.changeScene(new RemoteWorldScene(ip.c_str(), 1234));
    }
    
}