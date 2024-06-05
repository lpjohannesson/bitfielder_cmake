#include "color.h"
#include <sstream>

using namespace bf;

float Color::parseByte(unsigned int value, int shift) {
    unsigned char shiftedValue = (unsigned char)(value >> shift * 8);

    return ((float)shiftedValue) / 255.0f;
}

glm::vec4 Color::parseHex(std::string hex) {
    unsigned int value;

    std::stringstream ss;
    ss << std::hex << hex;
    ss >> value;

    return {
        parseByte(value, 2),
        parseByte(value, 1),
        parseByte(value, 0),
        1.0f
    };
}