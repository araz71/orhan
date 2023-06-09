#ifndef _REDIS_DATABASE_H_
#define _REDIS_DATABASE_H_

#include <Utility.h>
#include <DatabaseInterface.h>

#include <hiredis/hiredis.h>

#include <optional>
#include <string>
#include <vector>
#include <utility>

namespace orhan {

class RedisReply;

class RedisDatabase : public DatabaseInterface {
public:
	using FieldValuePair = std::pair<std::string, std::string>;

	RedisDatabase(const RedisDatabase&) = delete;
	RedisDatabase(const RedisDatabase&&) = delete;
	void operator=(const RedisDatabase&) = delete;

	bool add_device(const uint32_t device_id, const DeviceInformation& device_inf) override;
	bool add_register(const uint32_t device_id, const RegisterID register_id,
			const RegisterTypes type, const RegisterAccess access) override;
	bool load_device(const uint32_t device_id, DeviceInformation& device_inf, RegisterList& registers) override;
	void update_register(const uint32_t device_id, const RegisterID register_id, const std::string& data) override;

	static RedisDatabase& get_instance();

private:
	RedisDatabase();

	redisContext* redis_context;

protected:
	void execute(const std::string& command);

	// Adds or changes field to/of device
	void add_field(const uint32_t device_id, const std::string& field_name,
		const std::string& value);

	void remove_field(const uint32_t device_id, const std::string& field_name);

	// Retrieves field value from given device
	std::optional<std::string> get_field(const uint32_t device_id, const std::string& field_name);

	std::optional<std::vector<FieldValuePair>> get_fields(const uint32_t device_id);
};

class RedisReply {
public:
	explicit RedisReply(void* reply)
	: m_reply(reinterpret_cast<redisReply*>(reply))
	{
	}

	RedisReply(const void* reply) {
		m_reply = reinterpret_cast<redisReply*>(const_cast<void*>(reply));
	}

	RedisReply()
	: m_reply(NULL)
	{
	}

	redisReply* operator->() {
		return m_reply;
	}

	bool operator==(void* arg) {
		if (arg == m_reply) {
			return true;
		}
		return false;
	}

	~RedisReply() {
		if (m_reply != NULL) {
			freeReplyObject(m_reply);
		}
	}

private:
	redisReply* m_reply;
};

}
#endif
