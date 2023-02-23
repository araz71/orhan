#include "Client.h"

#include <vector>
#include <memory>

#include <string.h>

using namespace std;

constexpr size_t Client::MAXIMUM_PACKET_LENGTH;

Client::Client(const int socket_desctiptor, const uint32_t ip_address) :
	socket_desctiptor(socket_desctiptor),
	ip_address(ip_address),
        serial_number(0)
{

}

bool Client::is_ready() {
    if (serial_number == 0) return false;
    
    return true;
}

bool Client::load(uint32_t deviceID) {
    // Check database. if found load all registers.
}

bool Client::check_registerID(Packet::Functions function, uint16_t registerID {
   auto& register_flags = registers.find(registerID);
        
   if (register_flags == registers.end()) return false;

   if ((function == Packet::Functions::READ_ACK || function == Packet::Functions::WRITE)
                   && !(register_flags & WRITEABLE)) return false;

   if (function == Packet::Functions::READ && !(register_flags & READABLE)) return false;

   return true;
}

bool Client::write_ack(uint16_t registerID) {
    auto& flags = write_queue.find(registerID);
    if (flags == write_queue.end()) return false;

    write_queue.erase(flags);
    return true;
}

void Client::read_ack(uint16_t registerID, string& data) {
    auto& flags = read_queue.find(registerID);
    if (flags == read_queue.end()) return false;

    // Update data stored in database
    read_queue.erase(flags);
    return true;
}

string& Client::read(uint16_t registerID) {
   // read from database and return value 
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

    Packet::analys(packet, size, this);
}
