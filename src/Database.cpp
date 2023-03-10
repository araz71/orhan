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

bool Database::load(uint32_t device_id, Client& client) {
	database_locker.lock();
	// Retrive data
	database_locker.unlock();
}

bool Database::add_device(uint32_t device_id) {

}
