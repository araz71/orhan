#ifdef _DATABASE_WRAPPER_INL_H_
#error "This file should include just in DatabaseWrapper.h"
#else
#define _DATABASE_WRAPPER_INL_H_

using namespace orhan;

template <typename T>
T& DatabaseWrapper<T>::database = T::get_instance();

template <typename T>
bool DatabaseWrapper<T>::add_device(const uint32_t deviceID, const DeviceInformation& device_inf) {
	return database.add_device(deviceID, device_inf);	
}

template <typename T>
bool DatabaseWrapper<T>::load_device(const uint32_t deviceID, DeviceInformation& device_inf,
		RegisterList& registers) {
	return database.load_device(deviceID, device_inf, registers);
}

template <typename T>
void DatabaseWrapper<T>::update_register(const uint32_t deviceID, const RegisterID regID,
		const std::string& value) {
	database.update_register(deviceID, regID, value);
}

template <typename T>
bool DatabaseWrapper<T>::add_register(const uint32_t deviceID, const RegisterID regID,
		const RegisterTypes type, const RegisterAccess access)
{
	return database.add_register(deviceID, regID, type, access);
}

template <typename T>
bool DatabaseWrapper<T>::remove_register(const uint32_t device_id, const RegisterID regID) {
	return database.remove_register(device_id, regID);
}

#endif
