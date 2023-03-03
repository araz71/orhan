#include <Packet.h>

#include <string>

using namespace std;
using namespace orhan;

string Packet::packet_heartbit(uint32_t deviceID) {
	return packet_make(deviceID, HEARTBIT, 0, string());
}

string& Packet::make_write_ack(uint32_t deviceID, RegisterID regID) {
	return packet_make(deviceID, WRITE_ACK, regID, string());
}

string& Packet::make_read_ack(uint32_t deviceID, RegisterID regID, string& data) {
	return packet_make(deviceID, READ_ACK, regID, data);
}

string& Packet::make_write(uint32_t deviceID, RegisterID regID, string& data) {
	return packet_make(deviceID, WRITE, regID, data);
}

string& Packet::make_read(uint32_t deviceID, RegisterID regID) {
	return packet_make(deviceID, regID, string());
}

string& Packet::make(uint32_t deviceID, Functions function, RegisterID regID, string& data) {
        uint8_t buffer[MAXIMUM_PACKET_SIZE];
        memset(buffer, 0, sizeof(buffer));
        
        PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

        header->serial_number = deviceID;
        header->function = function;
        header->register_number = regID;
        header->len = data.length();

        memcpy(buffer + sizeof(PacketHeader), data.c_str(), data.length());

        uint16_t checksum = 0;
        for (size_t i = 0; i < (sizeof(PacketHeader) + data.length()); i++)
			checksum += buffer[i];

        return string(reinterpret_cast<char*>(buffer), sizeof(PacketHeader) + data.length());
}
