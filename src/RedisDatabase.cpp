#include "RedisDatabase.h"
#include "Utility.h"

#include <string>
#include <optional>
#include <exception>
#include <iostream>

#include <string.h>

using namespace std;
using namespace orhan;

RedisDatabase::RedisDatabase() {
	redis_context = redisConnect("127.0.0.1", 6379);
	if (redis_context == NULL) {
		throw runtime_error("Can not connect to the redis");
	}
}

RedisDatabase& RedisDatabase::get_instance() {
	static RedisDatabase db{};
	return db;
}

void RedisDatabase::add_field(const string& key, const std::string& field_name,
		const std::string& value)
{
	RedisReply reply(redisCommand(redis_context, "hset %b %s %b", key.c_str(), key.length()
				, field_name.c_str(), value.c_str(), value.length()));

	if (!reply.is_integer()) {
		throw runtime_error("Error at redis reply for add_field");
	}
}

void RedisDatabase::remove_field(const string& key, const std::string& field_name) {
	RedisReply reply = redisCommand(redis_context, "hdel %b %s", key.c_str(), key.length(), field_name.c_str());
	
	if (!reply.is_integer()) {
		throw runtime_error("Error at redis reply type for remove_field");
	}
}

optional<string> RedisDatabase::get_field(const string& key, const std::string& field_name) {
	RedisReply reply = redisCommand(redis_context, "hget %b %s", key.c_str(), key.length(), field_name.c_str());

	auto value = reply.is_string();
	return value;
}

optional<FieldValuePairList> RedisDatabase::get_fields(const string& key) {
	RedisReply reply = redisCommand(redis_context, "hgetall %b", key.c_str(), key.length());

	auto value = reply.is_array(true);
	return value;
}

optional<FieldValuePairList> RedisDatabase::get_keys(const string& pattern) {
	RedisReply reply = redisCommand(redis_context, "KEYS %b", pattern.c_str(), pattern.length());
	return reply.is_array(false);
}

void RedisDatabase::remove_key(const string& key) {
	RedisReply reply = redisCommand(redis_context, "del %b", key.c_str(), key.length());
}

bool RedisDatabase::add_device(const uint32_t device_id, const DeviceInformation& device_inf) {
	RedisReply reply = redisCommand(redis_context, "hset %d Inf %b", device_id, &device_inf, sizeof(DeviceInformation));
	return true;
}

bool RedisDatabase::remove_device(const uint32_t device_id) {
	remove_key(to_string(device_id));
	// Also remove all registers
	return true;
}

bool RedisDatabase::add_register(const uint32_t device_id, const RegisterID register_id,
		const RegisterTypes type, const RegisterAccess access)
{
	RedisReply reply = redisCommand(redis_context, "hset %d_%d type %s access %s",
			device_id, register_id,
			convert_register_type_to_string(type).c_str(),
			convert_register_access_to_string(access).c_str());

	if (reply.is_integer()) {
		return true;
	}	

	return false;
}

void RedisDatabase::remove_register(const uint32_t device_id, const RegisterID register_id) {
	const string key = to_string(device_id) + "_" + to_string(register_id);
	remove_key(key);
}

bool RedisDatabase::load_device(const uint32_t device_id, DeviceInformation& device_inf,
		RegisterMap& registers)
{
	auto deviceID = to_string(device_id);
	auto device = get_fields(deviceID);
	if (device) {
		for (auto& field_value : device.value()) {
			auto& [field_name, value] = field_value;
			if (field_name == "Inf") {
				if (value.size() != sizeof(device_inf)) {
					return false;
				}
				memcpy(&device_inf, reinterpret_cast<uint8_t*>(const_cast<char*>(value.c_str())), sizeof(device_inf));
			}
		}

		deviceID += "_*";
		auto device_reg_ids = get_keys(deviceID);
		if (device_reg_ids) {
			for (auto device_reg_id : device_reg_ids.value()) {
				// Get register whole identifiers
				auto regid = device_reg_id.first;
				regid = regid.substr(regid.find_first_of("_") + 1, regid.length());
				RegisterID regID = static_cast<RegisterID>(std::stoi(regid));
				auto reg_informations = get_fields(device_reg_id.first);
				if (reg_informations) {
					RegisterTypes reg_type;
					RegisterAccess reg_access;
					string reg_value;

					for (auto reg_inf : reg_informations.value()) {
						auto& [field_name, value] = reg_inf;
						if (field_name == "type") {
							convert_string_to_register_type(value, reg_type);
						} else if (field_name == "access") {
							convert_string_to_register_access(value, reg_access);
						} else if (field_name == "value") {
							reg_value = value;
						}
					}

					registers[regID] = {regID, reg_type, reg_access, reg_value};	
				}
			}
		}
	} else {
		return false;
	}
}

void RedisDatabase::update_register(const uint32_t device_id, const RegisterID register_id,
		const string& data)
{
	string key = to_string(device_id) + "_" + to_string(register_id);
	add_field(key, "value", data);
}
