#ifndef SQLITE_DATABASE_H_
#define SQLITE_DATABASE_H_

#include <string>
#include <optional>
#include <mutex>
#include <unordered_map>
#include <string>
#include <utility>

#include <sqlite3.h>

#include "DatabaseInterface.h"
#include "Client.h"

namespace orhan {

class SqliteDatabase : public DatabaseInterface {
public:
	using DatabaseInterface::DATABASE_ADDRESS;
	using DatabaseInterface::DEVICE_TABLE_CREATOR;
	using DatabaseInterface::Row;
	using DatabaseInterface::Rows;

	SqliteDatabase(const SqliteDatabase&) = delete;
	SqliteDatabase(const SqliteDatabase&&) = delete;
	void operator=(const SqliteDatabase&) = delete;	

	static SqliteDatabase& get_instance();

	bool load_device(const uint32_t device_id, DeviceInformation& device_inf,
			RegisterList &registers) override;

	/// See DatabaseInterface::add_device(const uint32_t);
	bool add_device(const uint32_t device_id, const DeviceInformation& device_inf) override;

	/// See DatabaseInterface::remove_device(const uint32_t);
	bool remove_device(const uint32_t device_id);
	
	/// See DatabaseInterface::add_register(const uint32_t, RegisterID);
	bool add_register(const uint32_t device_id, const RegisterID register_id, 
			const RegisterTypes type, const RegisterAccess access) override;

	void update_register(const uint32_t device_id, const RegisterID register_id, const std::string& data) override;

private:
	sqlite3 *sqlite_db;

	SqliteDatabase();

	static Rows retrieved_rows;
	static int retrieve_callback(void *data, int argc, char **argv, char **azColName) {
		Row row;
		for (int i = 0; i < argc; i++)
			row.emplace_back(std::make_pair(std::string(azColName[i]), std::string(argv[i])));

		retrieved_rows.push_back(row);

		return 0;
	}

protected:
	void execute(const std::string& command) override;
};

}
#endif
