#include "client_player_system.h"
#include "world/entity/systems/entity_system_impl.h"
#include "../components/client_player_component.h"
#include "world/entity/components/body_component.h"
#include <iostream>

using namespace bf;

void ClientPlayerSystem::update(World &world) {
    auto view = world.entities.registry.view<const ClientPlayerComponent, BodyComponent>();

    glm::vec2 movement = { 
        (float)client->clientInput.right.pressed - (float)client->clientInput.left.pressed,
        (float)client->clientInput.down.pressed - (float)client->clientInput.up.pressed
    };

    for (auto [entity, body] : view.each()) {
        body.velocity += movement * 4.0f * engine->getDeltaTime();
    }
}