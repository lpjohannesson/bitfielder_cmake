#pragma once
#include <vector>

namespace bf {
    class Animation {
    public:   
        std::vector<int> sequence;
        float duration;
        bool loops;

        int getFrame(float time) const;

        Animation(const std::vector<int> sequence, float duration, bool loops);
    };
}