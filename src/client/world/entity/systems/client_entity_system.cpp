#include "client_entity_system.h"

using namespace bf;

void ClientEntitySystem::loadScene(WorldScene &scene) {
    this->scene = &scene;
}