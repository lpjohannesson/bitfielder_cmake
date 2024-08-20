#include "random.h"

using namespace bf;

Random::Random() : randomInt(0, INT_MAX), randomFloat(0.0f, 1.0f) {
    randomEngine.seed(rand());
}