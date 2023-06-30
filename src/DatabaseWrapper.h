#ifndef _DATABASE_WRAPPER_H_
#define _DATABASE_WRAPPER_H_

#include "Utility.h"

#include <string>
#include <iostream>

namespace orhan
{
template <typename T>
class DatabaseWrapper {
public:
	DatabaseWrapper(const DatabaseWrapper&) = delete;
	DatabaseWrapper(DatabaseWrapper&) = delete;
	DatabaseWrapper& operator=(const DatabaseWrapper&) = delete;

	static bool add_device(const uint32_t deviceID, const DeviceInformation& device_inf);
	static bool remove_device(const uint32_t deviceID); 

	static bool load_device(const uint32_t deviceID, DeviceInformation& device_inf,
			RegisterMap& registers);

	static void update_register(const uint32_t deviceID, const RegisterID regID,
			const std::string& value);

	static bool add_register(const uint32_t deviceID, const RegisterID regID,
			const RegisterTypes type, const RegisterAccess access);

	static bool remove_register(const uint32_t deviceID, const RegisterID regID);
private:
	static T& database;

	DatabaseWrapper()
	{
	}
};

#include "DatabaseWrapper-inl.h"
}
#endif
