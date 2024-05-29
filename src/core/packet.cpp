#include "packet.h"
#include <winsock.h>

using namespace bf;

void Packet::write(const char *value, int size) {
    // Append data
    data.insert(data.end(), value, value + size);
}

char* Packet::read(int size) {
    char* dataPtr = data.data() + dataPosition;

    // Move offset forward
    dataPosition += size;

    // Get pointer to data with offset
    return dataPtr;
}

Packet& Packet::operator<<(bool value) {
    // Convert to byte
    char valueByte = value;
    write(&valueByte, sizeof(valueByte));

    return *this;
}

Packet& Packet::operator>>(bool &value) {
    char *valuePtr = read(sizeof(char));
    value = *valuePtr;

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
    int *valuePtr = (int*)read(sizeof(value));

    // Convert to host value
    value = ntohl(*valuePtr);

    return *this;
}

Packet& Packet::operator<<(float value) {
    write((char*)&value, sizeof(value));

    return *this;
}

Packet& Packet::operator>>(float &value) {
    float *valuePtr = (float*)read(sizeof(value));
    value = *valuePtr;

    return *this;
}

Packet& Packet::operator<<(glm::vec2 value) {
    return operator<<(value.x) << value.y;
}

Packet& Packet::operator>>(glm::vec2 &value) {
    return operator>>(value.x) >> value.y;
}