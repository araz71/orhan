#ifndef _DATABASE_WRAPPER_H_
#define _DATABASE_WRAPPER_H_

#include "Utility.h"

#include <string>

namespace orhan
{
template <typename T>
class DatabaseWrapper {
public:
	DatabaseWrapper(const DatabaseWrapper&) = delete;
	DatabaseWrapper(DatabaseWrapper&) = delete;
	DatabaseWrapper& operator=(const DatabaseWrapper&) = delete;

	static bool add_device(const uint32_t deviceID, const DeviceInformation& device_inf);

	static bool load_device(const uint32_t deviceID, DeviceInformation& device_inf,
			RegisterList& registers);

	static void update_register(const uint32_t deviceID, const RegisterID regID,
			const std::string& value);

	static bool add_register(const uint32_t deviceID, const RegisterID regID,
			const RegisterTypes type, const RegisterAccess access);

private:
	static T& database;

	DatabaseWrapper()
	{
	}
};

#include "DatabaseWrapper-inl.h"
}
#endif
