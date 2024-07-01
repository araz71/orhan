pp
#include <iostream>
#include <mysql/jdbc.h>

int main() {
    try {
        sql::Driver *driver = get_driver_instance();
        sql::Connection *con = driver->connect("tcp://localhost:3306", "user", "password");
        con->setSchema("test");

        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT 'Hello, MySQL' AS _message");
        while (res->next()) {
            std::cout << "\t... MySQL replies: " << res->getString("_message") << std::endl;
        }
        delete res;
        delete stmt;
        delete con;

    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }

    std::cout << "Done." << std::endl;
    return EXIT_SUCCESS;
}