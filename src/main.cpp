#include "Server.h"
#include "SqliteDatabase.h"

#include <time.h>
#include <stdlib.h>

using namespace std;
using namespace orhan;

int main() {
	const uint16_t port = 9000;
	Server orhan(port);
   
    srand(time(NULL));
	SqliteDatabase& db = SqliteDatabase::get_instance();
	db.add_device(1);//rand());
	db.add_register(1, 10, RegisterTypes::TYPE_UINT8_T, RegisterAccess::ACCESS_READ_WRITE);
	db.add_register(1, 10, RegisterTypes::TYPE_UINT16_T, RegisterAccess::ACCESS_READ);

	printf("Orhan started on port %d\r\n", port);
	while (1) {
	
	}	

	return 1;
}
