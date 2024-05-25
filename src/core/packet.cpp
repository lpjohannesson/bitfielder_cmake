#include "packet.h"
#include <winsock.h>

using namespace bf;

Packet& Packet::write(const char *value, int size) {
    // Append data
    data.insert(data.end(), value, value + size);
    return *this;
}

Packet& Packet::read(char *&value, int size) {
    // Set pointer to data with offset
    value = data.data() + dataPosition;

    // Move offset forward
    dataPosition += size;

    return *this;
}

Packet& Packet::operator>>(int value) {
    // Convert to network value
    int networkValue = htonl(value);

    // Write to packet
    write((char*)&networkValue, sizeof(value));

    return *this;
}

Packet& Packet::operator<<(int &value) {
    // Read pointer to network value
    int* networkValue;
    read((char*&)networkValue, sizeof(value));

    // Convert to host value
    value = ntohl(*networkValue);

    return *this;
}