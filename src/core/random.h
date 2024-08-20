#pragma once
#include <random>

namespace bf {
    class Random {
    public:
        std::default_random_engine randomEngine;
        std::uniform_int_distribution<int> randomInt;
        std::uniform_real_distribution<float> randomFloat;

        Random();
    };
}