#ifndef UTILITY_H_
#define UTILITY_H_

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

}

#endif
