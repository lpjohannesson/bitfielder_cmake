#include "packet.h"
#include <winsock.h>

using namespace bf;

Packet& Packet::write(const char *value, int size) {
    // Append data
    data.insert(data.end(), value, value + size);
    return *this;
}

Packet& Packet::read(char *&valuePtr, int size) {
    // Set pointer to data with offset
    valuePtr = data.data() + dataPosition;

    // Move offset forward
    dataPosition += size;

    return *this;
}

Packet& Packet::operator<<(int value) {
    // Convert to network value
    int networkValue = htonl(value);

    // Write to packet
    write((char*)&networkValue, sizeof(value));

    return *this;
}

Packet& Packet::operator>>(int &value) {
    // Read pointer to network value
    int *valuePtr;
    read((char*&)valuePtr, sizeof(value));

    // Convert to host value
    value = ntohl(*valuePtr);

    return *this;
}

Packet& Packet::operator<<(float value) {
    write((char*)&value, sizeof(value));

    return *this;
}

Packet& Packet::operator>>(float &value) {
    float *valuePtr;
    read((char*&)valuePtr, sizeof(value));

    value = *valuePtr;

    return *this;
}

Packet& Packet::operator<<(glm::vec2 value) {
    return operator<<(value.x) << value.y;
}

Packet& Packet::operator>>(glm::vec2 &value) {
    return operator>>(value.x) >> value.y;
}