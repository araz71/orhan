#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

#include <stdlib.h>
#include <string.h>

using namespace std;

vector<int> client_descriptors;
mutex client_descriptors_mutex;

void accepter(int server_descriptor) {
	while (1) {
		int new_client = accept(server_descriptor, NULL, 0);
		if (new_client != -1) {
			printf("new client\r\n");
			client_descriptors_mutex.lock();
			client_descriptors.push_back(new_client);
			client_descriptors_mutex.unlock();
		}
	}
}			

int main() {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		printf("Error\r\n");
	}

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9000);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
		printf("Can not bind on port 9000\r\n");
	}

	listen(fd, 1);

	struct sockaddr_in client_addr;
	
	thread server_listener(accepter, fd);

	char buffer[1024];
	while(1) {
		this_thread::sleep_for(chrono::milliseconds(10));
		client_descriptors_mutex.lock();
		for (auto client_fd : client_descriptors) {
			int size = recv(client_fd, buffer, 1024, MSG_DONTWAIT);
			if (size > 0) {
				for (int i = 0; i < size; i++) {
					printf("%c", buffer[i]);
				}
				printf("\r\n");
			}
	
		}
		client_descriptors_mutex.unlock();
	}
	return 1;
}

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

	/*
 	 * Functions :
 	 * Read			0
 	 * Write		1
 	 * HeardBit		2
 	 * Write ACK		3
 	 * Read ACK		4
 	 */
	if (header->function > 4) return false;

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

void packet_fill(uint32_t serial, 
