#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "gfx/core/texture.h"

namespace bf {
    class ShadowBuffer {
    private:
        GLuint glFramebuffer;

    public:
        inline GLuint getGLFramebuffer() const { return glFramebuffer; }

        Texture texture;

        void updateSize(glm::ivec2 size);

        ShadowBuffer();
        ~ShadowBuffer();
    };
}