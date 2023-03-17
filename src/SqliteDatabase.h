#ifndef SQLITE_DATABASE_H_
#define SQLITE_DATABASE_H_

#include <string>
#include <optional>
#include <mutex>
#include <unordered_map>

#include <sqlite3.h>

#include "DatabaseInterface.h"
#include "Client.h"

namespace orhan {

class SqliteDatabase : public DatabaseInterface {
 public:
    using DatabaseInterface::DATABASE_ADDRESS;
	using DatabaseInterface::DEVICE_TABLE_CREATOR;
	using DatabaseInterface::DEVICE_ADDING_COMMAND;

    SqliteDatabase(const SqliteDatabase&) = delete;
    SqliteDatabase(const SqliteDatabase&&) = delete;
    void operator=(const SqliteDatabase&) = delete;	

    static SqliteDatabase& get_instance();

	/// See DatabaseInterface::load_device(uint32_t, std::unordered_map<RegisterID, uint8_t>)
    bool load_device(uint32_t device_id, std::unordered_map<RegisterID, uint8_t>& register_map);

	/// See DatabaseInterface::add_device(uint32_t);
    bool add_device(uint32_t device_id);

	/// See DatabaseInterface::add_register(uint32_t, RegisterID);
    bool add_register(uint32_t device_id, RegisterID register_id);
 private:
    sqlite3 *sqlite_db;
    SqliteDatabase();
};

}
#endif
