#include "Database.h"
#include "Client.h"

using namespace std;
using namespace orhan;

Database& Database::get_instance() {
	static Database db;

	return db;
}

Database::Database() {

}

bool Database::load_device(uint32_t device_id, unordered_map<RegisterID, uint8_t>& client) {
	database_locker.lock();
	// Retrive data
	database_locker.unlock();
}

bool Database::add_device(uint32_t device_id) {

}

bool Database::add_register(uint32_t device_id, RegisterID register_id) {

}
