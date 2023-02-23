#include <Packet.h>

#include <string>

using namespace std;

string Packet::packet_heartbit(uint32_t deviceID) {
        return packet_make(deviceID, HEARTBIT, 0, string());
}

string& packet_write_ack(uint32_t deviceID, uint16_t registerID) {
        return packet_make(deviceID, WRITE_ACK, registerID, string());
}

string& packet_read_ack(uint32_t deviceID, uint16_t registerID, string& data) {
        return packet_make(deviceID, READ_ACK, registerID, data);
}

string& packet_write(uint32_t deviceID, uint16_t registerID, string& data) {
        return packet_make(deviceID, WRITE, registerID, data);
}

string& packet_read(uint32_t deviceID, uint16_t registerID) {
        return packet_make(deviceID, registerID, string());
}

string& Packet::packet_make(uint32_t deviceID, Functions function, uint16_t registerID, string& data) {
        uint8_t buffer[256];
        memset(buffer, 0, sizeof(buffer));
        
        PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

        header->serial_number = deviceID;
        header->function = function;
        header->register_number = registerID;
        header->len = data.length();

        memcpy(buffer + sizeof(PacketHeader), data.c_str(), data.length());

        uint16_t checksum = 0;
        for (size_t i = 0; i < (sizeof(PacketHeader) + data.length()); i++)
                checksum += buffer[i];

        return string(reinterpret_cast<char*>(buffer), sizeof(PacketHeader) + data.length());
}
