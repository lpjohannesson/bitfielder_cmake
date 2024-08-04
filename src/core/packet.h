#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>


namespace bf {
	class Packet {
    public:
        std::vector<uint8_t> data;
        size_t dataPosition = 0;

        bool ended() const;
        bool nextSizeAvailable(size_t size) const;

        uint8_t *getNextData();

        void write(const uint8_t *value, size_t size);
        uint8_t* read(size_t size);

        Packet& operator<<(bool value);
        Packet& operator>>(bool &value);

        Packet& operator<<(uint16_t value);
        Packet& operator>>(uint16_t &value);

        Packet& operator<<(int16_t value);
        Packet& operator>>(int16_t &value);

        Packet& operator<<(uint32_t value);
        Packet& operator>>(uint32_t &value);

        Packet& operator<<(int32_t value);
        Packet& operator>>(int32_t &value);

        Packet& operator<<(glm::ivec2 value);
        Packet& operator>>(glm::ivec2 &value);

        Packet& operator<<(float value);
        Packet& operator>>(float &value);

        Packet& operator<<(glm::vec2 value);
        Packet& operator>>(glm::vec2 &value);

        Packet& operator<<(std::string value);
        Packet& operator>>(std::string &value);

        void compressPacket(Packet &result);
        bool decompressPacket(size_t maxSize, Packet &result);
	};
}