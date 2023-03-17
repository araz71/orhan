#include "SqliteDatabase.h"
#include "Client.h"

using namespace std;
using namespace orhan;

SqliteDatabase& SqliteDatabase::get_instance() {
	static SqliteDatabase db{};
    
	return db;
}

SqliteDatabase::SqliteDatabase() {
    const char* database_address = "clients.db";

}

bool SqliteDatabase::load_device(uint32_t device_id, unordered_map<RegisterID, uint8_t>& client) {
	lock.lock();
	// Retrive data
	lock.unlock();
}

bool SqliteDatabase::add_device(uint32_t device_id) {

}

bool SqliteDatabase::add_register(uint32_t device_id, RegisterID register_id) {

}
