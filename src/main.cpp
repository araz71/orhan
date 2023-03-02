#include "Server.h"
#include "Client.h"

#include <map>

using namespace std;
using namespace orhan;

int main() {
	const uint16_t port = 9000;
	Server orhan(port);

	printf("Orhan started on port %d\r\n", port);
	while (1) {
	
	}	
	return 1;
}
/*

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
