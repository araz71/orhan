#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/optional.hpp>

#include "Packet.h"
#include "Utility.h"

#include <time.h>
#include <stdint.h>

namespace orhan
{

class Client {
 private:
	static constexpr size_t MAXIMUM_PACKET_LENGTH = 256;

	typedef enum {
            WRITEABLE = 0x01,
            READABLE = 0x02
    } RegisterAccessFlags;

	int socket_desctiptor;
	
	uint32_t ip_address;

	uint32_t serial_number;

	time_t last_communication_timestamp;

    std::unordered_map<orhan::RegisterID, uint16_t> write_queue;
    std::unordered_map<orhan::RegisterID, uint16_t> read_queue;

	std::unordered_map<orhan::RegisterID, RegisterAccessFlags> registers;

 public:
	Client(const int socket_desctiptor, const uint32_t ip_address);

	bool is_ready();

    /**
     * Searchs client ID inside database
     *
     * @param deviceID Device serial number
     * @return 
    bool load(uint32_t deviceID);

    /**
     * Search regID in clients registers and stores data.
     *
     * @param regID registers number
     * @param data Data which should store in clients register table
     * @return True if register id found and false if not found
     */
    bool write(orhan::RegisterID regID, std::string& data);
    
    /**
     * Checks if client requested write function
     *
     * @param regID register which requested to write
     * @return True if register found in write queue otherwise False
     */
    bool write_ack(orhan::RegisterID regID);
    
    /**
     * Reads register data from clients register data table
     *
     * @param regID number of register which should read from clients data table
     * @return boost::none if register not found. otherwise data of requested register.
     */
    boost::optional<std::string> read(orhan::RegisterID regID);
   
    /**
     * Writes data incomes from device into registers data field.
     *
     * @param regID number of register which requested to read
     * @return True if reading register found in read queue. otherwise returns False.
     */
    bool read_ack(orhan::RegisterID regID, std::string& data);

    /**
     * Checks if register is found in client register table or not
     *
     * @param function Function which will do on register
     * @param regID number of register
     * @return True if register found and function has access. otherwise false
     */
    bool check_registerID(orhan::Functions function, orhan::RegisterID regID);
	
    // Sets clients serial number
    void set_serial_number(const uint32_t serial_number);

    // Updates last client communication
	void update_communication();

    // Returns clients serial number
	uint32_t get_serial_number();
	
    // Returns last clients communication timestamp
	time_t get_last_communication_timestamp();
	
	uint32_t get_ip_address();
	
	int get_descriptor();
	
	bool add_packet(uint8_t* packet, size_t len, std::string& response);
};

}
#endif
