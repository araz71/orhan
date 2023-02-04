#include <string.h>

#include <time.h>
#include <stdint.h>

class Client {
 private:
	int socket_desctiptor;
	
	uint32_t ip_address;

	uint32_t serial_number;

	time_t last_communication_timestamp;

 public:
	Client(const int socket_desctiptor, const uint32_t ip_address);
	
	void set_serial_number(const uint32_t serial_number);
	void update_communication();

	uint32_t get_serial_number();
	time_t get_last_communication_timestamp();
	uint32_t get_ip_address();
	int get_descriptor();
};
