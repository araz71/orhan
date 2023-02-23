#include <stdint.h>


class Packet {
 private:
	typedef struct {
		uint32_t serial_number;

		uint8_t function;
		uint16_t register_number;
		uint16_t len;
		uint16_t checksum;
	} PacketHeader;

        string& packet_make(uint32_t deviceID, functions function, uint16_t registerID, string data);
        
        string& packet_write(uint32_t deviceID, uint16_t registerID, string& data);
        string& packet_write_ack(uint32_t deviceID, uint16_t registerID);

        string& packet_read(uint32_t deviceID, uint16_t registerID);
        string& packet_read_ack(uint32_t deviceID, uint16_t registerID, string& data);

        string& packet_heartbit(uint32_t deviceID);

 public:
	typedef enum {
		READ,
		WRITE,
		HEARTBIT,
		WRITE_ACK,
		READ_ACK,
                UNKNOWN_FUNCTION,
	} Functions;

	static bool analys(uint8_t* packet, size_t packet_len, Client& client) {
		if (packet_size < sizeof(PacketHeader)) return false;

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

		Functions function = header->function;
		if (function == HEARTBIT)
			client.update_communication();

		else if (function == WRITE_ACK) {
			// return register id for client
//			client.write_ack(header->register_number);

		} else if (function == READ_ACK) {
			// Copy data to register number and notify to client
//			client.read_ack(header->register_number, data, data_len);
	
		} else if (function == WRITE) {
                        //clinet.write(header->register_number, data);
                        //
                } else if (function == READ) {
  /*                      string& result;
			if (client.read(header->register_number, result)) {
                                
                        } else {
                        
                        }*/
		}

                return true;
	}
}


