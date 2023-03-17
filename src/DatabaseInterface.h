#ifndef DATABASE_INTERFACE_H_
#define DATABASE_INTERFACE_H_

#include <unordered_map>
#include <mutex>

#include "Utility.h"

namespace orhan
{

class DatabaseInterface {
 public:
    const char* DATABASE_ADDRESS = "clients.db";
    const char* DEVICE_TABLE_CREATOR = "CREATE TABLE IF NOT EXISTS DEVICE(deviceID INTEGER PRIMARY KEY)";

    virtual bool add_device(uint32_t device_id) = 0;
	virtual bool add_register(uint32_t device_id, RegisterID register_id) = 0;
	virtual bool load_device(uint32_t device_id, std::unordered_map<RegisterID, uint8_t>& register_map) = 0;

 protected:
    std::mutex lock;
};

}

#endif
