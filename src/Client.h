#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include "Packet.h"
#include "Utility.h"

#include <time.h>
#include <stdint.h>

namespace orhan
{

class Client {
 private:
	static constexpr size_t MAXIMUM_PACKET_LENGTH = 256;

	typedef enum {
            WRITEABLE = 0x01,
            READABLE = 0x02
    } RegisterAccessFlags;

	int socket_desctiptor;
	
	uint32_t ip_address;

	uint32_t serial_number;

	time_t last_communication_timestamp;

    std::unordered_map<orhan::RegisterID, uint16_t> write_queue;
    std::unordered_map<orhan::RegisterID, uint16_t> read_queue;

	std::unordered_map<orhan::RegisterID, RegisterAccessFlags> registers;

 public:
	Client(const int socket_desctiptor, const uint32_t ip_address);

	bool is_ready();

    bool load(uint32_t deviceID);

    void write_ack(orhan::RegisterID regID);
    void read_ack(orhan::RegisterID regID, std::string& data);
    std::string& read(orhan::RegisterID regID);

    bool check_registerID(orhan::Functions function, orhan::RegisterID regID);
	void set_serial_number(const uint32_t serial_number);
	
	void update_communication();

	uint32_t get_serial_number();
	
	time_t get_last_communication_timestamp();
	
	uint32_t get_ip_address();
	
	int get_descriptor();
	
	bool add_packet(uint8_t* packet, size_t len);
};

}
#endif
