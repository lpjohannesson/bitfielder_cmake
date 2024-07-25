#include "camera.h"
#include "client/scenes/world_scene.h"
#include "core/direction.h"
#include "core/game_time.h"

using namespace bf;

CameraPlayerInfo Camera::getPlayerInfo(const WorldScene &scene) {
    const entt::registry &entityRegistry = scene.world.entities.registry;
    entt::entity player = scene.clientContent.player;

    LocalPlayerComponent localPlayer = entityRegistry.get<LocalPlayerComponent>(player);
    glm::vec2 position = entityRegistry.get<PositionComponent>(player).position;
    BodyComponent body = entityRegistry.get<BodyComponent>(player);
    AimComponent aim = entityRegistry.get<AimComponent>(player);

    return { position + body.size * 0.5f, (float)aim.aim, body.isOnFloor, localPlayer.blockTime > 0.0f };
}

void Camera::setZoom(float zoom) {
    this->zoom = zoom;
    zoomTransform = glm::scale(glm::mat4(1.0f), glm::vec3(zoom * 16.0f));
}

void Camera::update(WorldScene &scene) {
    CameraPlayerInfo playerInfo = getPlayerInfo(scene);

    // Camera Y
    if (playerInfo.isModifyingBlock) {
        targetY = playerInfo.position.y;
        Direction::targetAxis(targetPosition.y, targetY, slowPanSpeedY * gameTime.getDeltaTime());
    }
    else {
        if (position.y < playerInfo.position.y) {
            // Always pan down
            targetPosition.y = targetY = playerInfo.position.y;
        }
        else {
            // Pan up if on floor
            if (playerInfo.isOnFloor) {
                targetY = playerInfo.position.y;
            }

            Direction::targetAxis(targetPosition.y, targetY, panSpeedY * gameTime.getDeltaTime());
        }
    }

    // Drag camera X, offset forward when dragged
    float dragMinX = playerInfo.position.x - dragDistanceX;
    float dragMaxX = playerInfo.position.x + dragDistanceX;

    float dragX = glm::clamp(targetPosition.x, dragMinX, dragMaxX);

    targetOffset.x = glm::clamp(targetOffset.x + (dragX - targetPosition.x) * dragSpeedX, -maxOffsetX, maxOffsetX);
    targetPosition.x = dragX;

    // Offset Y by aim
    Direction::targetAxis(targetOffset.y, playerInfo.aim * aimHeight, slowPanSpeedY * gameTime.getDeltaTime());

    // Smooth positions to targets
    float deltaTime = gameTime.getDeltaTime();

    position = glm::lerp(targetPosition, position, glm::pow(smooth, deltaTime));
    offset = glm::lerp(targetOffset, offset, glm::pow(smooth, deltaTime));

    glm::vec2 translation = -(position + offset);

    transform = zoomTransform * glm::translate(glm::mat4(1.0f), glm::vec3(translation, 0.0f));
}

void Camera::start(WorldScene &scene) {
    position = getPlayerInfo(scene).position;
    targetPosition = position;
    targetY = position.y;

    update(scene);
}

Camera::Camera() {
    dragDistanceX = 1.0f;
    dragSpeedX = 0.3f;
    maxOffsetX = 2.5f;
    panSpeedY = 8.0f;
    slowPanSpeedY = 4.0f;
    aimHeight = 2.0f;
    smooth = glm::pow(2.0f, -16.0f);
}