#pragma once
#include <vector>

namespace bf {
	class Packet {
        std::vector<char> data;
        int dataPosition = 0;

    public:
        Packet& write(const char *value, int size);
        Packet& read(char *&value, int size);

        Packet& operator>>(int value);
        Packet& operator<<(int &value);
	};
}