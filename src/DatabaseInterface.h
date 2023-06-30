#ifndef DATABASE_INTERFACE_H_
#define DATABASE_INTERFACE_H_

#include <unordered_map>
#include <mutex>
#include <utility>
#include <vector>
#include <utility>

#include "Utility.h"

namespace orhan
{

class DatabaseInterface {
public:
	using Row = std::vector<std::pair<std::string, std::string>>;
	using Rows = std::vector<Row>;

	const char* DATABASE_ADDRESS = "clients.db";
	const char* DEVICE_TABLE_CREATOR = "CREATE TABLE IF NOT EXISTS"
		" DEVICE(deviceID INTEGER PRIMARY KEY, Type TEXT, build_date TEXT, firmware TEXT, register_date TEXT)";

	/**
	 * Adds new device into DEVICE table. also creates dev_device_id table contains registers.
	 *
	 * @param device_id New device id
	 * @param device_inf Decice information
	 * @return True if device added and false if it was created before.
	 */
	virtual bool add_device(const uint32_t device_id, const DeviceInformation& device_inf) = 0;

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
	virtual bool add_register(const uint32_t device_id, const RegisterID register_id,
		const RegisterTypes type, const RegisterAccess access) = 0;

	/**
	 * @brief Loads device's register's maps from database
	 * @param device_id Interested device
	 * @param device_inf Device Information
	 * @param register_map Result of registers downloaded.
	 * @return True if device found. otherwise false.
	 */
	virtual bool load_device(const uint32_t device_id, DeviceInformation& device_inf, RegisterMap& registers) = 0;
	virtual bool remove_device(const uint32_t device_id) = 0;

	/**
	 * @brief Updates value of interested register
	 * @param device_id Interested device
	 * @param register_id Interested register
	 * @param data Data
	 */
	virtual void update_register(const uint32_t device_id, const RegisterID register_id, const std::string& data) = 0;
	virtual void remove_register(const uint32_t device_id, const RegisterID register_id) = 0;

protected:
	std::mutex lock;
};

}

#endif
