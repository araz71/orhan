#ifndef SQLITE_DATABASE_H_
#define SQLITE_DATABASE_H_

#include <string>
#include <optional>
#include <mutex>
#include <unordered_map>
#include <string>

#include <sqlite3.h>

#include "DatabaseInterface.h"
#include "Client.h"

namespace orhan {

class SqliteDatabase : public DatabaseInterface {
public:
    using DatabaseInterface::DATABASE_ADDRESS;
	using DatabaseInterface::DEVICE_TABLE_CREATOR;
	using DatabaseInterface::Rows;

    SqliteDatabase(const SqliteDatabase&) = delete;
    SqliteDatabase(const SqliteDatabase&&) = delete;
    void operator=(const SqliteDatabase&) = delete;	

    static SqliteDatabase& get_instance();

	/// See DatabaseInterface::load_device(const uint32_t, std::unordered_map<RegisterID, uint8_t>)
    bool load_device(const uint32_t device_id, vector<unoredered_map<RegisterID, Register> > &register_map) override;

	/// See DatabaseInterface::add_device(const uint32_t);
    bool add_device(const uint32_t device_id) override;

	/// See DatabaseInterface::add_register(const uint32_t, RegisterID);
    bool add_register(const uint32_t device_id, RegisterID register_id, RegisterTypes type, RegisterAccess access) override;

	void update_register(const uint32_t device_id, RegisterID register_id, const std::string& data) override;

private:
    sqlite3 *sqlite_db;

	SqliteDatabase();

	static Rows retrieved_rows;
	static int retrieve_callback(void *data, int argc, char **argv, char **azColName) {
        unordered_map<string, string> row;
        for (int i = 0; i < argc; i++)
            row[azColName[i]] = (argv[i] == NULL ? "" : argv[i]);

        retrieved_rows.push_back(row);
		return 0;
	}

protected:
	void execute(const std::string& command) override;
};

}
#endif
