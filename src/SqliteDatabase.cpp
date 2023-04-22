#include <iostream>

#include "SqliteDatabase.h"
#include "Client.h"

using namespace std;
using namespace orhan;

SqliteDatabase::Rows SqliteDatabase::retrieved_rows;

SqliteDatabase& SqliteDatabase::get_instance() {
	static SqliteDatabase db{};
 
	return db;
}

void SqliteDatabase::execute(const string& command) {
	retrieved_rows.clear();

	const int ret = sqlite3_exec(sqlite_db, command.c_str(), SqliteDatabase::retrieve_callback, NULL, NULL);
	if (ret != SQLITE_OK)
		throw runtime_error("Sqlite(" + to_string(ret) + ") : Failed to execute " + command);
}

SqliteDatabase::SqliteDatabase() {
    const int ret = sqlite3_open(DATABASE_ADDRESS, &sqlite_db);
    if (ret != SQLITE_OK)
        throw runtime_error("Sqlite : Can't open database");

	execute(DEVICE_TABLE_CREATOR);
}

bool SqliteDatabase::load_device(const uint32_t device_id, unordered_map<RegisterID, uint8_t>& client) {
	lock.lock();
	// Retrive data
    execute("SELECT * FROM DEVICE WHERE deviceID='" + to_string(device_id) + "'");
    if (retrieved_rows.size() == 0)
        return false;

    execute("SELECT * FROM dev_" + to_string(device_id) + " WHERE deviceID='" + to_string(device_id) + "'");
    for (auto& row : retrieved_rows) {
        auto& [regID, regFlags, regValue] = row;
        cout << "RegID : " << regID << " - Flags : " << regFlags << endl;
    }
    lock.unlock();
}

bool SqliteDatabase::add_device(const uint32_t device_id) {
    lock.lock();

	string deviceID = to_string(device_id);
	execute("SELECT * FROM DEVICE WHERE deviceID='" + deviceID + "'");

	if (retrieved_rows.size() == 0) {
		execute("INSERT INTO DEVICE(deviceID) values('" + deviceID + "')");	
		execute("CREATE TABLE IF NOT EXISTS dev_" + deviceID +
				"(registerID INTEGER PRIMARY KEY,flags INTEGER,value TEXT)");
	} else {
		cout << "Device has defined before" << endl;
	}

	lock.unlock();

	return true;
}

bool SqliteDatabase::add_register(const uint32_t device_id, RegisterID register_id, RegisterType type, RegisterAccess access) {
	execute("SELECT * FROM dev_" + to_string(device_id) +
			" WHERE registerID='" + to_string(register_id) + "'");
	
	if (retrieved_rows.size() == 0) 
		execute("INSERT INTO dev_" + to_string(device_id) +
				"(registerID,type,access,value) VALUES(" + 
					"'" + to_string(register_id) + "'," +
					"'" + to_string(static_cast<int>(type)) + "'," +
					"'" + to_string(static_cast<int>(access) + "','0')"));
	else
		cout << "RegisterID is found" << endl;
}

void SqliteDatabase::update_register(const uint32_t device_id, RegisterID register_id, const string& data) {
	execute("UPDATE dev_" + to_string(device_id) + " SET VALUE='" + 
			data + "' WHERE registerID='" + to_string(register_id) + "'");
}
