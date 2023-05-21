#ifndef PACKET_H_
#define PACKET_H_

#include <string>
#include <optional>

#include <stdlib.h>
#include <string.h>

#include "Utility.h"
#include "Client.h"

namespace orhan
{

class Packet {
private:

public:
	static void make(uint32_t deviceID, orhan::Functions function, orhan::RegisterID regID,
		std::optional<std::string> data, std::string& buffer)
	{
		PacketHeader header;

		buffer.resize(MAXIMUM_PACKET_SIZE);
		buffer.clear();

		memset(&header, 0, sizeof(PacketHeader));

		header.serial_number = deviceID;
		header.function = function;
		header.register_number = regID;
		if (data)
			header.len = data.value().length();

		uint16_t& checksum = header.checksum;
		uint8_t* pointer_to_header = reinterpret_cast<uint8_t*>(&header);
		for (size_t i = 0; i < sizeof(PacketHeader); i++)
			checksum += pointer_to_header[i];

		if (data) {
			for (size_t i = 0; i < data.value().length(); i++)
			checksum += data.value()[i];
		}

		buffer.append(reinterpret_cast<char*>(&header), sizeof(PacketHeader)); 

		if (data)
			buffer.append(data.value());
	}

	static void make_write(const uint32_t deviceID, const orhan::RegisterID regID, const std::string& data,
			std::string& buffer)
	{
		make(deviceID, Functions::WRITE, regID, data, buffer);
	}

	static void make_write_ack(const uint32_t deviceID, const orhan::RegisterID regID, std::string& buffer) {
		make(deviceID, Functions::WRITE_ACK, regID, std::string(), buffer);
	}

	static void make_read(const uint32_t deviceID, const orhan::RegisterID regID, std::string& buffer) {
		make(deviceID, Functions::READ, regID, std::string(), buffer);
	}

	static void make_read_ack(const uint32_t deviceID, const orhan::RegisterID regID,
			const std::string& data, std::string& buffer)
	{
		make(deviceID, Functions::READ_ACK, regID, data, buffer);
	}

	static void make_heartbit(const uint32_t deviceID, std::string& buffer) {
		make(deviceID, Functions::HEARTBIT, 0, std::string(), buffer);
	}

	template <typename T>
	static bool analys(const uint8_t* packet, const size_t packet_len, std::string& response, T& client) {
		if (packet_len < sizeof(PacketHeader))
			return false;

		PacketHeader *header;
		uint8_t* data;
		size_t data_len;

		header = reinterpret_cast<PacketHeader*>(const_cast<uint8_t*>(packet));
		if (header->function >= UNKNOWN_FUNCTION)
			return false;

		// Check for deviceID
		//		if (!client.is_ready() && !client.load(header->serial_number)) return false;

		// Check for register number and function access
		if (!client.check_registerID(static_cast<orhan::Functions>(header->function), header->register_number))
			return false;

		data = const_cast<uint8_t*>(packet) + sizeof(PacketHeader);
		data_len = packet_len - sizeof(PacketHeader);

		orhan::Functions function = static_cast<orhan::Functions>(header->function);
		if (function == orhan::Functions::HEARTBIT)
			client.update_communication();

		else if (function == orhan::Functions::WRITE_ACK) {
			// return register id for client
			client.write_ack(header->register_number);

		} else if (function == orhan::Functions::READ_ACK) {
			// Copy data to register number and notify to client
			client.read_ack(header->register_number, std::string(reinterpret_cast<char*>(data), data_len));

		} else if (function == orhan::Functions::WRITE) {
			client.write(header->register_number,
					std::string(reinterpret_cast<char*>(data), data_len));
			make_write_ack(header->serial_number, header->register_number, response);

		} else if (function == orhan::Functions::READ) {
			auto data = client.read(header->register_number);
			if (data)
				make_read_ack(header->serial_number, header->register_number, data.value(), response);
			else
				return false;
		}
		return true;
	}
};

}
#endif
