
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
        string& packet_read(uint32_t deviceID, uint16_t registerID);
        string& packet_write_ack(uint32_t deviceID, uint16_t registerID);
        string& packet_read_ack(uint32_t deviceID, uint16_t registerID, string& data);
        string& packet_heartbit(uint32_t deviceID);

 public:
	typedef enum {
		READ,
		WRITE,
		HEARTBIT,
		WRITE_ACK,
		READ_ACK
	} functions;


	static bool request_open(uint8_t* packet, size_t packet_len, Client& client) {
		if (packet_size < sizeof(PacketHeader)) return false;

		PacketHeader *header;
		uint8_t* data;
		size_t data_len;

		header = reinterpret_cast<PacketHeader*>(packet);
		data = packet + sizeof(PacketHeader);
		data_len = packet_len - sizeof(PacketHeader);

		if (header->function > 4) return false;

	        // Search for client seial number;

		uint8_t function = header->function;
		if (function == HEARTBIT)
			client.update_communication();

		else if (function == WRITE_ACK) {
			// return register id for client
//			client.write_ack(header->register_number);

		} else if (function == READ_ACK) {
			// Copy data to register number and notify to client
//			client.read_ack(header->register_number, data, data_len);
	
		} else if (function == WRITE) {


                } else if (function == READ) {
  /*                      string& result;
			if (client.read(header->register_number, result)) {
                                
                        } else {
                        
                        }*/
		}

                return true;
	}
}

string& Packet::packet_heartbit(uint32_t deviceID) {
        return packet_make(deviceID, HEARTBIT, 0, string());
}

string& Packet::packet_make(uint32_t deviceID, functions function, uint16_t registerID, string& data) {
        uint8_t buffer[256];
        memset(buffer, 0, sizeof(buffer));
        PacketHeader* header = reinterpret_cast<PacketHeader>(buffer);

        header->serial_number = deviceID;
        header->function = function;
        header->register_number = registerID;
        header->len = data.length();
        memcpy(buffer + sizeof(PacketHeader), data.c_str(), data.length());

        return string(reinterpret_cast<char>(buffer), sizeof(PacketHeader) + data.length());
}
