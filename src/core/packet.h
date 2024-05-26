#pragma once
#include <vector>

namespace bf {
	class Packet {
    public:
        std::vector<char> data;
        int dataPosition = 0;

        Packet& write(const char *value, int size);
        Packet& read(char *&value, int size);

        Packet& operator<<(int value);
        Packet& operator>>(int &value);
	};
}