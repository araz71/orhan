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

	add_field(1, "f1", "v1");
	add_field(1, "f2", "v2");
	add_field(1, "f3", "v3");

	remove_field(1, "f3");
	// Select database here
}

RedisDatabase& RedisDatabase::get_instance() {
	static RedisDatabase db{};
	return db;
}

void RedisDatabase::add_field(const uint32_t device_id, const std::string& field_name,
		const std::string& value)
{
	RedisReply reply(redisCommand(redis_context, "hset %d %s %b", device_id, field_name.c_str(), value.c_str(), value.length()));

	if (!reply.is_integer()) {
		throw runtime_error("Error at redis reply for add_field");
	}
}

void RedisDatabase::remove_field(const uint32_t device_id, const std::string& field_name) {
	RedisReply reply = redisCommand(redis_context, "hdel %d %s", device_id, field_name.c_str());
	
	if (reply.is_integer()) {
		throw runtime_error("Error at redis reply type for remove_field");
	}
}

optional<string> RedisDatabase::get_field(const uint32_t device_id, const std::string& field_name) {
	RedisReply reply = redisCommand(redis_context, "hget %d %s", device_id, field_name.c_str());

	auto value = reply.is_string();
	return value;
}

optional<vector<RedisDatabase::FieldValuePair>> RedisDatabase::get_fields(const uint32_t device_id) {
	RedisReply reply = redisCommand(redis_context, "hgetall %d", device_id);

	auto value = reply.is_array();
	return value;
}

bool RedisDatabase::add_device(const uint32_t device_id, const DeviceInformation& device_inf) {
	RedisReply reply = redisCommand(redis_context, "hset %d Inf %b", device_id, &device_inf, sizeof(DeviceInformation));
}

bool RedisDatabase::add_register(const uint32_t device_id, const RegisterID register_id,
		const RegisterTypes type, const RegisterAccess access)
{
}

bool RedisDatabase::load_device(const uint32_t device_id, DeviceInformation& device_inf,
		RegisterList& registers)
{

}

void RedisDatabase::update_register(const uint32_t device_id, const RegisterID register_id,
		const string& data)
{

}

void RedisDatabase::execute(const std::string& command) {
	

}
