#ifndef DATABASE_H_
#define DATABASE_H_

#include <string>
#include <optional>
#include <mutex>
#include <unordered_map>

#include "DatabaseInterface.h"
#include "Client.h"

namespace orhan {

class Database : public DatabaseInterface {
	public:
		Database(const Database&) = delete;
		Database(const Database&&) = delete;
		void operator=(const Database&) = delete;	

		static Database& get_instance();

		bool load_device(uint32_t device_id, std::unordered_map<RegisterID, uint8_t>& register_map);
		
		bool add_device(uint32_t device_id);

		bool add_register(uint32_t device_id, RegisterID register_id);
	private:
		std::mutex database_locker;

		Database();
};

}
#endif
