#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace bf {
	class Packet {
    public:
        std::vector<char> data;
        int dataPosition = 0;

        bool ended() const;

        void write(const char *value, int size);
        char* read(int size);

        Packet& operator<<(bool value);
        Packet& operator>>(bool &value);

        Packet& operator<<(int value);
        Packet& operator>>(int &value);

        Packet& operator<<(glm::ivec2 value);
        Packet& operator>>(glm::ivec2 &value);

        Packet& operator<<(float value);
        Packet& operator>>(float &value);

        Packet& operator<<(glm::vec2 value);
        Packet& operator>>(glm::vec2 &value);
	};
}