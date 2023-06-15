#include "RedisDatabase.h"

#include <string>
#include <optional>
#include <exception>
#include <iostream>

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

optional<vector<RedisDatabase::FieldValuePair>> RedisDatabase::get_fields(const string& key) {
	RedisReply reply = redisCommand(redis_context, "hgetall %b", key.c_str(), key.length());

	auto value = reply.is_array();
	return value;
}

void RedisDatabase::remove_key(const string& key) {
	redisCommand(redis_context, "hdel %b", key.c_str(), key.length());
}

bool RedisDatabase::add_device(const uint32_t device_id, const DeviceInformation& device_inf) {
	RedisReply reply = redisCommand(redis_context, "hset %d Inf %b", device_id, &device_inf, sizeof(DeviceInformation));
	return true;
}

bool RedisDatabase::remove_device(const uint32_t device_id) {
	remove_key(to_string(device_id));
	return true;
}

bool RedisDatabase::add_register(const uint32_t device_id, const RegisterID register_id,
		const RegisterTypes type, const RegisterAccess access)
{
	RedisReply reply = redisCommand(redis_context, "hset %d_%d type %s access %s",
			device_id, register_id,
			convert_register_type_to_string(type), convert_register_access_to_string(access));

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
		RegisterList& registers)
{
	auto device = get_fields(to_string(device_id));
	if (device) {
		vector<string> register_ids;
		for (auto& field_value : device) {
			auto& [field_name, value] = field_value;
			if (field_name == "Inf") {
				if (value.size() != sizeof(device_inf)) {
					return false;
				}
				memcpy(&device_inf, reinterpret_cast<uint8_t*>(value.c_str()), sizeof(device_inf));
			} else if (field_name == "Registers") {
				split_strings(value, "\n", register_ids);	
			}
		}

		auto registers = get_fields
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
