#include "SqliteDatabase.h"
#include "Client.h"

using namespace std;
using namespace orhan;

SqliteDatabase& SqliteDatabase::get_instance() {
	static SqliteDatabase db{};
    
	return db;
}

SqliteDatabase::SqliteDatabase() {
    int ret = sqlite3_open(DATABASE_ADDRESS, &sqlite_db);
    if (ret != SQLITE_OK)
        throw runtime_error("Sqlite : Can't open database");

    char* errmsg;
    ret = sqlite3_exec(sqlite_db, DEVICE_TABLE_CREATOR, NULL, NULL, &errmsg);
    if (ret != SQLITE_OK)
        throw runtime_error("Sqlite : Error at creating clients table");
}

bool SqliteDatabase::load_device(uint32_t device_id, unordered_map<RegisterID, uint8_t>& client) {
	lock.lock();
	// Retrive data
	lock.unlock();
}

bool SqliteDatabase::add_device(uint32_t device_id) {
    lock.lock();
    char *errmsg;
	string command = "INSERT INTO DEVICE(deviceID) values('" + to_string(device_id) + "')";
	int ret = sqlite3_exec(sqlite_db, command.c_str(), NULL, NULL, &errmsg);
	if (ret != SQLITE_OK)
		throw runtime_error("Sqlite : Error at adding new device");
}

bool SqliteDatabase::add_register(uint32_t device_id, RegisterID register_id) {

}
