#pragma once
#include "../renderers/block_renderer.h"

namespace bf {
    struct BlockRendererComponent {
        BlockRenderer *renderer = nullptr;
    };
}