#include "packet.h"
#include <zlib.h>

#ifdef _WIN32
	#include <winsock.h>
#else
	#include <arpa/inet.h>
#endif

using namespace bf;

#define INT_PACKET_DEF(INT_TYPE, HTON, NTOH)\
    Packet& Packet::operator<<(INT_TYPE value) {\
        /* Convert to network value */\
        INT_TYPE networkValue = HTON(value);\
        \
        /* Write to packet */\
        write((uint8_t*)&networkValue, sizeof(value));\
        \
        return *this;\
    }\
    \
    Packet& Packet::operator>>(INT_TYPE &value) {\
        /*Read pointer to network value*/\
        INT_TYPE *nextData = (INT_TYPE*)read(sizeof(value));\
        \
        if (nextData == nullptr) {\
            return *this;\
        }\
        \
        /* Convert to host value */\
        value = NTOH(*nextData);\
        \
        return *this;\
    }

bool Packet::ended() const {
    return dataPosition >= data.size();
}

bool Packet::nextSizeAvailable(size_t size) const {
    size_t maxSize = data.size() - dataPosition;

    return size <= maxSize;
}

uint8_t *Packet::getNextData() {
    return data.data() + dataPosition;
}

void Packet::write(const uint8_t *value, size_t size) {
    if (value == nullptr) {
        return;
    }
    
    data.insert(data.end(), value, value + size);
}

uint8_t* Packet::read(size_t size) {
    if (!nextSizeAvailable(size)) {
        return nullptr;
    }

    uint8_t* nextData = getNextData();

    // Move offset forward
    dataPosition += size;

    // Get pointer to data with offset
    return nextData;
}

Packet& Packet::operator<<(bool value) {
    // Convert to byte
    uint8_t valueByte = value;
    write(&valueByte, sizeof(valueByte));

    return *this;
}

Packet& Packet::operator>>(bool &value) {
    uint8_t *nextData = read(sizeof(uint8_t));

    if (nextData == nullptr) {
        return *this;
    }

    value = *nextData;

    return *this;
}

INT_PACKET_DEF(uint16_t, htons, ntohs)
INT_PACKET_DEF(int16_t, htons, ntohs)
INT_PACKET_DEF(uint32_t, htonl, ntohl)
INT_PACKET_DEF(int32_t, htonl, ntohl)

Packet& Packet::operator<<(glm::ivec2 value) {
    return operator<<(value.x) << value.y;
}

Packet& Packet::operator>>(glm::ivec2 &value) {
    return operator>>(value.x) >> value.y;
}

Packet& Packet::operator<<(float value) {
    write((uint8_t*)&value, sizeof(value));

    return *this;
}

Packet& Packet::operator>>(float &value) {
    float *nextData = (float*)read(sizeof(value));

    if (nextData == nullptr) {
        return *this;
    }

    value = *nextData;

    return *this;
}

Packet& Packet::operator<<(glm::vec2 value) {
    return operator<<(value.x) << value.y;
}

Packet& Packet::operator>>(glm::vec2 &value) {
    return operator>>(value.x) >> value.y;
}

Packet& Packet::operator<<(std::string value) {
    // Write length, then data
    operator<<((uint32_t)value.length());
    write((uint8_t*)value.data(), value.length());

    return *this;
}

Packet& Packet::operator>>(std::string &value) {
    uint32_t size;
    operator>>(size);

    uint8_t *nextData = read(size);

    if (nextData == nullptr) {
        return *this;
    }

    value.reserve(size);
    value = std::string((char*)nextData, size);

    return *this;
}

void Packet::compressPacket(Packet &result) {
    // Create compressed buffer
    uLongf destLen = compressBound((uLong)data.size());

    std::vector<uint8_t> resultData;
    resultData.reserve(destLen);

    compress(resultData.data(), &destLen, (uint8_t*)data.data(), (uLong)data.size());

    // Write compressed size, decompressed size, and data
    result << (uint32_t)destLen << (uint32_t)data.size();
    result.write(resultData.data(), destLen);
}

bool Packet::decompressPacket(size_t maxSize, Packet &result) {
    // Get sizes with overflow check
    uint32_t compressedSize, decompressedSize;
    operator>>(compressedSize) >> decompressedSize;

    if (decompressedSize > maxSize) {
        return false;
    }

    uint8_t *nextData = read(compressedSize);

    if (nextData == nullptr) {
        return false;
    }

    // Write result directly
    result.data.resize(result.data.size() + decompressedSize);

    uLongf destLen = decompressedSize;
    int compressResult = uncompress(result.getNextData(), &destLen, nextData, compressedSize);

    if (compressResult != Z_OK) {
        return false;
    }

    return true;
}