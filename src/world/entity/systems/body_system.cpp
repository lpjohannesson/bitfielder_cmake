#include "body_system.h"
#include "entity_system_impl.h"
#include "../components/position_component.h"
#include "../components/body_component.h"
#include "engine/engine.h"

using namespace bf;

void BodySystem::update(World &world) {
    auto view = world.entities.registry.view<PositionComponent, BodyComponent>();

    for (auto [entity, position, body] : view.each()) {
        position.position += body.velocity * engine->getDeltaTime();
    }
}