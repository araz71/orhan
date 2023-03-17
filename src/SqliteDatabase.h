#ifndef SQLITE_DATABASE_H_
#define SQLITE_DATABASE_H_

#include <string>
#include <optional>
#include <mutex>
#include <unordered_map>

#include "DatabaseInterface.h"
#include "Client.h"

namespace orhan {

class SqliteDatabase : public DatabaseInterface {
 public:
    SqliteDatabase(const SqliteDatabase&) = delete;
    SqliteDatabase(const SqliteDatabase&&) = delete;
    void operator=(const SqliteDatabase&) = delete;	

    static SqliteDatabase& get_instance();

    bool load_device(uint32_t device_id, std::unordered_map<RegisterID, uint8_t>& register_map);
		
    bool add_device(uint32_t device_id);

    bool add_register(uint32_t device_id, RegisterID register_id);
 private:
        SqliteDatabase();
};

}
#endif
