#include "camera.h"
#include "client/scenes/world_scene.h"
#include "core/direction.h"
#include "core/game_time.h"

using namespace bf;

CameraPlayerInfo Camera::getPlayerInfo(const WorldScene &scene) {
    const entt::registry &entityRegistry = scene.world.entities.registry;
    entt::entity player = scene.clientContent.player;

    glm::vec2 position = entityRegistry.get<PositionComponent>(player).position;
    BodyComponent body = entityRegistry.get<BodyComponent>(player);

    return { position + body.size * 0.5f, body.isOnFloor };
}

void Camera::update(WorldScene &scene) {
    CameraPlayerInfo playerInfo = getPlayerInfo(scene);

    // Drag camera X, offset forward when dragged
    float dragMinX = playerInfo.position.x - dragDistanceX;
    float dragMaxX = playerInfo.position.x + dragDistanceX;

    float dragX = glm::clamp(position.x, dragMinX, dragMaxX);

    offset.x = glm::clamp(offset.x + (dragX - position.x) * offsetSpeedX, -maxOffsetX, maxOffsetX);
    position.x = dragX;

    // Camera Y
    if (position.y < playerInfo.position.y) {
        // Always pan down
        position.y = targetY = playerInfo.position.y;
    }
    else {
        // Pan up if on floor
        if (playerInfo.isOnFloor) {
            targetY = playerInfo.position.y;
        }

        Direction::targetAxis(position.y, targetY, panSpeedY * gameTime.getDeltaTime());
    }

    transform = glm::translate(glm::mat4(1.0f), glm::vec3(-(position + offset), 0.0f));
}

void Camera::start(WorldScene &scene) {
    position = getPlayerInfo(scene).position;
    targetY = position.y;
}

Camera::Camera() {
    dragDistanceX = 1.5f;
    offsetSpeedX = 0.5f;
    maxOffsetX = 2.5f;
    panSpeedY = 8.0f;
}