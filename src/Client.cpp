#include "Client.h"

#include <vector>
#include <memory>

#include <string.h>

using namespace std;
using namespace orhan;

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
    return true;
}

bool Client::check_registerID(Functions function, RegisterID regID) {
   auto& register_flags = registers.find(regID);
        
   if (register_flags == registers.end()) return false;

   if ((function == Functions::READ_ACK || function == Functions::WRITE)
	   && !(register_flags->second & WRITEABLE)) return false;

   if (function == Functions::READ && !(register_flags->second & READABLE)) return false;

   return true;
}

bool Client::write_ack(RegisterID regID) {
    auto& flags = write_queue.find(regID);
    if (flags == write_queue.end()) return false;

    write_queue.erase(flags);
    return true;
}

bool Client::read_ack(RegisterID regID, string& data) {
    auto& flags = read_queue.find(regID);
    if (flags == read_queue.end()) return false;

    // Update data stored in database
    read_queue.erase(flags);
    return true;
}

boost::optional<string> Client::read(RegisterID regID) {
   // read from database and return value
   return boost::none;
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

bool Client::add_packet(uint8_t* packet, size_t size, string& response) {
	if (size > MAXIMUM_PACKET_LENGTH)
		return false;

    Packet::analys(packet, size, response, *this);
}
