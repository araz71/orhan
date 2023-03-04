#ifndef PACKET_H_
#define PACKET_H_
#include <string>

#include "Utility.h"
#include "Client.h"

namespace orhan
{

class Packet {
 private:

 public:
	void make(uint32_t deviceID, orhan::Functions function, orhan::RegisterID regID, std::string data, std::string& buffer);
    
    void make_write(uint32_t deviceID, orhan::RegisterID regID, std::string& data, std::string& buffer);
    void make_write_ack(uint32_t deviceID, orhan::RegisterID regID, std::string& buffer);

    void make_read(uint32_t deviceID, orhan::RegisterID regID, std::string& buffer);
    void make_read_ack(uint32_t deviceID, orhan::RegisterID regID, std::string& data, std::string& buffer);

    void make_heartbit(uint32_t deviceID, std::string& buffer);

	static bool analys(uint8_t* packet, size_t packet_len, orhan::Client& client) {
		if (packet_len < sizeof(PacketHeader)) return false;

		PacketHeader *header;
		uint8_t* data;
		size_t data_len;

		header = reinterpret_cast<PacketHeader*>(packet);
		if (header->function >= UNKNOWN_FUNCTION) return false;
				
		// Check for deviceID
		if (!client.is_ready() && !client.load(header->serial_number)) return false;

		// Check for register number and function access
		if (!client.check_registerID(header->function, header->register_number)) return false;

		data = packet + sizeof(PacketHeader);
		data_len = packet_len - sizeof(PacketHeader);

		orhan::Functions function = static_cast<orhan::Functions>(header->function);
		if (function == orhan::Functions::HEARTBIT)
			client.update_communication();

		else if (function == orhan::Functions::WRITE_ACK) {
		// return register id for client
	//		client.write_ack(header->register_number);

		} else if (function == orhan::Functions::READ_ACK) {
			// Copy data to register number and notify to client
	//		client.read_ack(header->register_number, data, data_len);
		
		} else if (function == orhan::Functions::WRITE) {
			//clinet.write(header->register_number, data);
			//
		} else if (function == orhan::Functions::READ) {
	/*                  std::string& result;
			if (client.read(header->register_number, result)) {
			} else {
			}*/
		}

		return true;
	}
};

}
#endif
