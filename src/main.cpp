#include "Server.h"
#include "SqliteDatabase.h"

using namespace std;
using namespace orhan;

int main() {
	const uint16_t port = 9000;
	Server orhan(port);
   
    SqliteDatabase::get_instance().add_device(1);

	printf("Orhan started on port %d\r\n", port);
	while (1) {
	
	}	
	return 1;
}
