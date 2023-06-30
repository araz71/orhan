#ifndef _REDIS_DATABASE_H_
#define _REDIS_DATABASE_H_

#include <Utility.h>
#include <DatabaseInterface.h>

#include <hiredis/hiredis.h>

#include <optional>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

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
	bool load_device(const uint32_t device_id, DeviceInformation& device_inf, RegisterMap& registers) override;
	bool remove_device(const uint32_t device_id) override; 
	void update_register(const uint32_t device_id, const RegisterID register_id, const std::string& data) override;
	void remove_register(const uint32_t device_id, const RegisterID register_id) override;

	static RedisDatabase& get_instance();

private:
	RedisDatabase();

	redisContext* redis_context;

protected:
	// Adds or changes field to/of key
	void add_field(const std::string& key, const std::string& field_name, const std::string& value);

	// Removes field from a key
	void remove_field(const std::string& key, const std::string& field_name);

	// Retrieves field value from given key
	std::optional<std::string> get_field(const std::string& key, const std::string& field_name);

	// Retrives all keys follows given pattern
	std::optional<FieldValuePairList> get_keys(const std::string& pattern);

	// Retrieves all fields and values from given key
	std::optional<FieldValuePairList> get_fields(const std::string& key);
	void remove_key(const std::string& key);
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

	explicit RedisReply()
	: m_reply(NULL)
	{
	}

	void operator=(void* reply) {
		m_reply = reinterpret_cast<redisReply*>(reply);
	}

	redisReply* operator->() {
		return m_reply;
	}

	bool operator()() {
		if (m_reply != NULL) {
			return true;
		}
		return false;
	}

	bool operator==(void* arg) {
		if (arg == m_reply) {
			return true;
		}
		return false;
	}

	bool is_status() {
		if (this->operator()()) {
			if (m_reply->type == REDIS_REPLY_STATUS) {
				return true;
			}
		}
		return false;
	}

	std::optional<int> is_integer(int expected_value = -1) {
		if (this->operator()()) {
			if (m_reply->type == REDIS_REPLY_INTEGER) {
				return std::optional<int>(m_reply->integer);
			}
		}
		return std::nullopt;	
	}

	std::optional<std::string> is_string() {
		if (this->operator()()) {
			if (m_reply->type == REDIS_REPLY_STRING) {
				return std::optional<std::string>(std::string(m_reply->str, m_reply->len));
			}
		}
		return std::nullopt;
	}

	std::optional<std::vector<FieldValuePair>> is_array(bool is_field_value) {
		if (this->operator()()) {
			if (m_reply->type == REDIS_REPLY_ARRAY) {
				std::vector<FieldValuePair> field_values;
				if (is_field_value) {
					for (size_t counter = 0; counter < m_reply->elements; counter += 2) {
						redisReply* field_name = m_reply->element[counter];
						redisReply* value = m_reply->element[counter + 1];
						field_values.push_back(std::make_pair(std::string(field_name->str, field_name->len),
									std::string(value->str, value->len)));
					}
					return field_values;
				} else {
					for (size_t counter = 0; counter < m_reply->elements; counter++) {
						redisReply* value = m_reply->element[counter];
						field_values.push_back(std::make_pair(std::string(value->str, value->len), ""));
					}
					return field_values;
				}
			} else {
				return std::nullopt;
			}
		}

		return std::nullopt;
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
