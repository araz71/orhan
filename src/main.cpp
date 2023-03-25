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
	db.add_register(1, 10);
	db.add_register(1, 10);
	db.add_register(1, 20);
	db.update_register(1, 10, "Salam");

	printf("Orhan started on port %d\r\n", port);
	while (1) {
	
	}	
	return 1;
}
