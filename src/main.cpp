#include "Server.h"
#include "Client.h"

#include <map>

using namespace std;

int main() {
	Server orhan(9000);	

	while (1) {
	
	}	
	return 1;
}
/*
typedef struct {
	uint32_t serial_number;

	uint8_t function;
	uint16_t register_number;
	uint16_t len;
	uint16_t checksum;
} PacketHeader;

typedef enum {
	READ,
	WRITE,
	HEARTBIT,
	WRITE_ACK,
	READ_ACK
};

bool request_open(uint8_t* packet, size_t packet_len) {
	if (packet_size < sizeof(PacketHeader)) return false;

	PacketHeader *header;
	header = reinterpret_cast<PacketHeader*>(packet);
*/
	/*
 	 * Functions :
 	 * Read			0
 	 * Write		1
 	 * HeardBit		2
 	 * Write ACK		3
 	 * Read ACK		4
 	 */
/*	if (header->function > 4) return false;

	// Search for client seial number;

	uint8_t function = header->function;
	if (function == HEARDBIT) {
		// Client::update_...
	} else if (function == WRITE_ACK) {
		// return register id for client
	} else if (function == READ_ACK) {
		// Copy data to register number and notify to client
	} else if (function == WRITE) {
		// Copy data to registee number and notify to device
		// Send WRITE_ACK to device
	} else if (function == READ) {
		// Send register value to device
	} 
}

void reg_write(uint32_t serial_number, uint16_t register_number, uint8_t* value, size_t value_len) {

}

void write_request(uint32_t serial, uint8_t* data, uint16_t size) {
	uint8_t packet[256];
	PacketHeader* header = reinterpret_cast<PacketHeader*>(packet);

	memset(packet, 0, sizeof(packet));

	header->serial_number = serial;
	header->function = WRITE;
	header->register_number = register_number;
	header->len = size;
}

void read_request(uint8_t serial, uint16_t register_number) {
	uint8_t packet[256];
	memset(packet, 0, sizeof(packet));

	PacketHeader* header = reinterpret_cast<PacketHeader*>(packet);

	header->serial_number = serial;
	header->function = READ;
	header->register_number = register_number;
}

void packet_fill(uint32_t serial, */
