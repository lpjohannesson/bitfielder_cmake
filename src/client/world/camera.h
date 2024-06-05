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
        float zoom = 1.0f;
        glm::mat4 transform, zoomTransform;

        static CameraPlayerInfo getPlayerInfo(const WorldScene &scene);

    public:
        inline float getZoom() const { return zoom; }
        inline glm::mat4 getTransform() const { return transform; }

        float dragDistanceX, offsetSpeedX, maxOffsetX, panSpeedY, blockPanSpeedY;
        
        glm::vec2 position{}, offset{};
        float targetY;

        void setZoom(float zoom);

        void update(WorldScene &scene);

        void start(WorldScene &scene);

        Camera();
    };
}