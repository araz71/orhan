#ifndef DATABASE_H_
#define DATABASE_H_

#include <string>
#include <optional>
#include <mutex>

#include "Client.h"

namespace orhan {

class Database {
	public:
		Database(const Database&) = delete;
		Database(const Database&&) = delete;
		void operator=(const Database&) = delete;	

		static Database& get_instance();

		bool load(uint32_t device_id, orhan::Client& client);
		
		bool add_device(uint32_t device_id);
	private:
		std::mutex database_locker;

		Database();
};

}
#endif
