#include "Client.h"
#include "Database.h"

#include <vector>
#include <memory>
#include <optional>
#include <tuple>

#include <string.h>

using namespace std;
using namespace orhan;

constexpr size_t Client::MAXIMUM_PACKET_LENGTH;

Client::Client(const int socket_desctiptor, const uint32_t ip_address) :
	socket_desctiptor(socket_desctiptor),
	ip_address(ip_address),
	serial_number(0)
{
	registers[DEVICE_LOGIN_REGISTER] = {DEVICE_LOGIN_REGISTER, RegisterTypes::TYPE_BINARY, RegisterAccess::ACCESS_WRITE, string()};
}

bool Client::is_ready() {
	if (serial_number == 0)
		return false;
    
	return true;
}

bool Client::load(const uint32_t deviceID) {
	// Check database. if found load all registers.
	return Database::load_device(deviceID, device_informations, registers);
}

bool Client::check_registerID(const Functions function, const RegisterID regID) {
	auto reg = registers.find(regID);

	if (reg == registers.end())
		return false;

	auto [id, type, access, value] = reg->second;
	if ((function == Functions::READ_ACK || function == Functions::WRITE)
		&& !(access & RegisterAccess::ACCESS_WRITE))
	{
		return false;
	}

	if (function == Functions::READ && !(access & RegisterAccess::ACCESS_READ))
		return false;

	return true;
}

bool Client::write_ack(const RegisterID regID) {
	const auto flags = write_queue.find(regID);
	if (flags == write_queue.end())
		return false;

	write_queue.erase(flags);
	return true;
}

bool Client::read_ack(const RegisterID regID, const string& data) {
	const auto flags = read_queue.find(regID);
	if (flags == read_queue.end())
		return false;

	// Update data stored in database
	read_queue.erase(flags);
	auto& reg = registers[regID];
	auto& [id, type, access, value] = reg;
	value = data;

	return true;
}

optional<string> Client::read(const RegisterID regID) {
	// read from database and return value
	return std::nullopt;
}

bool Client::write(const RegisterID regID, const string& data) {
	auto reg = registers.find(regID);

	if (reg == registers.end())
		return false;

	string& value = reg->second.value;
	value = data;

	return true;
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
