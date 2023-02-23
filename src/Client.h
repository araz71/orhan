#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "Packet.h"

#include <vector>
#include <memory>
#include <map>

#include <time.h>
#include <stdint.h>

class Client {
 private:
	static constexpr size_t MAXIMUM_PACKET_LENGTH = 256;

	int socket_desctiptor;
	
	uint32_t ip_address;

	uint32_t serial_number;

	time_t last_communication_timestamp;

        std::unordered_map<uint16_t> write_queue;
        std::unordered_map<uint16_t> read_queue;

        typedef enum {
                WRITEABLE = 0x01,
                READABLE = 0x02
        } RegisterAccessFlags;

        std::unordered_map<uint16_t, RegisterAccessFlags> registers;

 public:
	Client(const int socket_desctiptor, const uint32_t ip_address);

        bool is_ready();
        
        bool load(uint32_t deviceID);

        void write_ack(uint16_t registerID);
        void read_ack(uint16_t registerID, string& data);
        string& read(uint16_t registerID);

        bool check_registerID(Packet::functions function, uint16_t registerID);
	void set_serial_number(const uint32_t serial_number);
	
	void update_communication();

	uint32_t get_serial_number();
	
	time_t get_last_communication_timestamp();
	
	uint32_t get_ip_address();
	
	int get_descriptor();
	
	bool add_packet(uint8_t* packet, size_t len);
};

#endif
