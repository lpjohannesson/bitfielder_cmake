#pragma once
#include <glm/glm.hpp>

namespace bf {
    class WorldScene;

    struct CameraPlayerInfo {
        glm::vec2 position;
        bool isOnFloor, isModifyingBlock;
    };

    class Camera {
    private:
        static CameraPlayerInfo getPlayerInfo(const WorldScene &scene);

        glm::mat4 transform;

    public:
        inline glm::mat4 getTransform() const { return transform; }

        float dragDistanceX, offsetSpeedX, maxOffsetX, panSpeedY, blockPanSpeedY;

        glm::vec2 position{}, offset{};
        float targetY;

        void update(WorldScene &scene);

        void start(WorldScene &scene);

        Camera();
    };
}