#include "RedisDatabase.h"

#include <string>
#include <exception>
#include <iostream>

using namespace std;
using namespace orhan;

void RedisDatabase::add_field(const uint32_t device_id, const std::string& field_name,
		const std::string& value)
{
	void* reply = redisCommand("hset %d %s %b", device_id, field_name.c_str(), value.c_str(), value.length());
	if (reply == NULL) {
		throw runtime_error("Can not add field to database.");
	}

	freeReplyObject(reply);
}

void RedisDatabase::remove_field(const uint32_t device_id, const std::string& field_name) {
	void* reply = redisCommand("hdel %d %s", device_id, field_name.c_str());
	if (reply == NULL) {
		throw runtime_error("Can not remove field from database");
	{
}

optional<string> RedisDatabase::get_field(const uint32_t device_id, const std::string& field_name) {
	void *reply = redisCommand("hget %d %s", device_id, field_name.c_str());
	if (reply == NULL) {
		throw runtime_error("Can not retrieve field from database");
}

vector<string, string> RedisDatabase::get_fields(const uint32_t device_id) {

}

RedisDatabase::RedisDatabase() {
	redis_context = redisConnect("127.0.0.1", 6379);
	if (redis_context == NULL) {
		throw runtime_error("Can not connect to the redis");
	}

	// Select database here
}

RedisDatabase& RedisDatabase::get_instance() {
	static RedisDatabase db{};
	return db;
}

bool RedisDatabase::add_device(const uint32_t device_id, const DeviceInformation& device_inf) {
	redisCommand(redis_context, "hset %d Inf %b", device_id, &device_inf, sizeof(DeviceInformation));
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
