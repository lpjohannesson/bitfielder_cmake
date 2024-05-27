#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace bf {
	class Packet {
    public:
        std::vector<char> data;
        int dataPosition = 0;

        Packet& write(const char *value, int size);
        Packet& read(char *&valuePtr, int size);

        Packet& operator<<(int value);
        Packet& operator>>(int &value);

        Packet& operator<<(float value);
        Packet& operator>>(float &value);

        Packet& operator<<(glm::vec2 value);
        Packet& operator>>(glm::vec2 &value);
	};
}