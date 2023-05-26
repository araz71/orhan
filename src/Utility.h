#ifndef UTILITY_H_
#define UTILITY_H_

#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

namespace orhan
{

#define MAXIMUM_PACKET_SIZE     	256
#define DEVICE_LOGIN_REGISTER		1024

using RegisterID = uint16_t;
using StringList = std::vector<std::string>;
using StringPair = std::pair<std::string, std::string>;

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
	READ_ACK,
	UNKNOWN_FUNCTION,
} Functions;

typedef enum {
	TYPE_UINT8_T,
	TYPE_UINT16_T,
	TYPE_UINT32_T,
	TYPE_INT8_T,
	TYPE_INT16_T,
	TYPE_INT32_T,
	TYPE_STRING,
	TYPE_BINARY
} RegisterTypes;

typedef enum {
	ACCESS_READ = 0x01,
	ACCESS_WRITE = 0x02,
	ACCESS_READ_WRITE = 0x03,
} RegisterAccess;

typedef struct {
	uint16_t regID;
	RegisterTypes type;
	RegisterAccess access;
	std::string value;
} Register;

typedef struct {
	std::string type;
	std::string firmware;
	std::string build_date;
	std::string register_date;
} DeviceInformation;

using RegisterList = std::unordered_map<RegisterID, Register>;

constexpr bool convert_string_to_register_type(std::string& string_type, RegisterTypes& register_type) {
	std::unordered_map<std::string, RegisterTypes> types_map;

	types_map["uint8"] = RegisterTypes::TYPE_UINT8_T;
	types_map["uint16"] = RegisterTypes::TYPE_UINT16_T;
	types_map["uint32"] = RegisterTypes::TYPE_UINT32_T;
	types_map["int8"] = RegisterTypes::TYPE_INT8_T;
	types_map["int16"] = RegisterTypes::TYPE_INT16_T;
	types_map["int32"] = RegisterTypes::TYPE_INT32_T;
	types_map["string"] = RegisterTypes::TYPE_STRING;
	types_map["raw"] = RegisterTypes::TYPE_BINARY;

	if (types_map.find(string_type) == types_map.end())
		return false;

	register_type = types_map[string_type];
	return true;
}

inline void split_strings(std::string base_string, std::string spliter, StringList& return_value) {
	while (base_string.find(spliter) != std::string::npos) {
		return_value.emplace_back(base_string.substr(0, base_string.find(spliter)));
		base_string = base_string.substr(base_string.find(spliter) + 1, base_string.length());
	}
	return_value.emplace_back(base_string);
}

}
#endif
