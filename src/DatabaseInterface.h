#ifndef DATABASE_INTERFACE_H_
#define DATABASE_INTERFACE_H_

#include <unordered_map>
#include <mutex>
#include <utility>
#include <vector>

#include "Utility.h"

namespace orhan
{

class DatabaseInterface {
public:
    using Rows = std::unordered_map<std::string, std::string>;

    const char* DATABASE_ADDRESS = "clients.db";
    const char* DEVICE_TABLE_CREATOR = "CREATE TABLE IF NOT EXISTS DEVICE(deviceID INTEGER PRIMARY KEY)";

	/**
	 * Adds new device into DEVICE table. also creates dev_device_id table contains registers.
	 *
	 * @param device_id New device id
	 * @return True if device added and false if it was created before.
	 */
    virtual bool add_device(const uint32_t device_id) = 0;

	/**
	 * Adds register to existed device
	 *
	 * @param device_id Interested device's
	 * @param register_id New register's ID
	 * @param type Type of register
	 * @param access Access level of register
	 * 
	 * @return True if register added and false if register was created before.
	 */
	virtual bool add_register(const uint32_t device_id, RegisterID register_id, RegisterType type, RegisterAccess access) = 0;
	virtual bool load_device(const uint32_t device_id, std::unordered_map<RegisterID, uint8_t>& register_map) = 0;
	virtual void update_register(const uint32_t device_id, RegisterID register_id, const std::string& data) = 0;
protected:
	std::mutex lock;

	virtual void execute(const std::string& command) = 0;
};

}

#endif
