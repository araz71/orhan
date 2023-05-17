#ifndef UTILITY_H_
#define UTILITY_H_

#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

namespace orhan
{

using RegisterID = uint16_t;

#define MAXIMUM_PACKET_SIZE     256

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

#define DEVICE_LOGIN_REGISTER		1024

}
#endif
