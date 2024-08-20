#pragma once
#include <glm/glm.hpp>
#include "sound/sound.h"

namespace bf {
    class WorldScene;

    struct CameraPlayerInfo {
        glm::vec2 position;
        float aim;
        bool isOnFloor, isModifyingBlock;
    };

    class Camera {
    private:
        float zoom = 1.0f;
        float verticalTimer = 0.0f;

        glm::mat4 transform, zoomTransform;

        static CameraPlayerInfo getPlayerInfo(const WorldScene &scene);

    public:
        inline float getZoom() const { return zoom; }
        inline glm::mat4 getTransform() const { return transform; }

        float smooth, dragDistanceX, dragSpeedX, maxOffsetX, panSpeedY, slowPanSpeedY, aimHeight, verticalTime, maxStickOffset;
        
        glm::vec2 position{}, targetPosition{}, offset{}, targetOffset{};
        float targetY;

        void setZoom(float zoom);

        void update(WorldScene &scene);

        void reset(WorldScene &scene);

        Camera();
    };
}