#include <Packet.h>

#include <string>

using namespace std;
using namespace orhan;

void Packet::make_heartbit(uint32_t deviceID, string& buffer) {
    make(deviceID, HEARTBIT, 0, string(), buffer);
}

void Packet::make_write_ack(uint32_t deviceID, RegisterID regID, string& buffer) {
    make(deviceID, WRITE_ACK, regID, string(), buffer);
}

void Packet::make_read_ack(uint32_t deviceID, RegisterID regID, string& data, string& buffer) {
	make(deviceID, READ_ACK, regID, data, buffer);
}

void Packet::make_write(uint32_t deviceID, RegisterID regID, string& data, string& buffer) {
    make(deviceID, WRITE, regID, data, buffer);
}

void Packet::make_read(uint32_t deviceID, RegisterID regID, string& buffer) {
	make(deviceID, regID, string(), buffer);
}

void Packet::make(uint32_t deviceID, Functions function, RegisterID regID, string& data, string& buffer) {
    PacketHeader header;

    buffer.resize(MAXIMUM_PACKET_SIZE);
    buffer.clear();
  
    memset(&header, 0, sizeof(PacketHeader));

    header.serial_number = deviceID;
    header.function = function;
    header.register_number = regID;
    header.len = data.length();

    uint16_t& checksum = header.checksum;
    uint8_t* pointer_to_header = reinterpret_cast<uint8_t*>(&header);
    for (size_t i = 0; i < sizeof(PacketHeader); i++)
        checksum += pointer_to_header[i];

    for (size_t i = 0; i < data.length(); i++)
        checksum += data[i];

    buffer.append(reinterpret_cast<char*>(&header), sizeof(PacketHeader)); 
    buffer.append(data);
}
