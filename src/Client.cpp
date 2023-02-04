#include "Client.h"

Client::Client(const int socket_desctiptor, const uint32_t ip_address) :
	socket_desctiptor(socket_desctiptor),
	ip_address(ip_address)
{
}

void Client::set_serial_number(const uint32_t serial_number) {
	Client::serial_number = serial_number;
}

uint32_t Client::get_serial_number() {
	return serial_number;
}

void Client::update_communication() {
	last_communication_timestamp = time(NULL);
}

time_t Client::get_last_communication_timestamp() {
	return last_communication_timestamp;
}

uint32_t Client::get_ip_address() {
	return ip_address;
}

int Client::get_descriptor() {
	return socket_desctiptor;
}
