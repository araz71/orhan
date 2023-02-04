#include "Client.h"

#include <vector>
#include <memory>

#include <string.h>

using namespace std;

constexpr size_t Client::MAXIMUM_PACKET_LENGTH;

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

bool Client::add_packet(uint8_t* packet, size_t size) {
	if (size > MAXIMUM_PACKET_LENGTH)
		return false;

	uint8_t* packet_buffer = new uint8_t[size];
	memcpy(packet_buffer, packet, size);

	packets.push_back(packet_buffer);

	return true;
}
