#include "Server.h"
#include "SqliteDatabase.h"

#include <iostream>

#include <time.h>
#include <stdlib.h>

using namespace std;
using namespace orhan;

int main() {
	const uint16_t port = 9000;
	Server orhan(port);

	srand(time(NULL));
	SqliteDatabase& db = SqliteDatabase::get_instance();

	DeviceInformation inf = {"I/O", "2.1", "May 10 23", "May 10 23"};
	db.add_device(1, inf);//rand());
	db.add_register(1, 10, RegisterTypes::TYPE_UINT8_T, RegisterAccess::ACCESS_READ_WRITE);
	db.add_register(1, 11, RegisterTypes::TYPE_UINT16_T, RegisterAccess::ACCESS_READ);

	DeviceInformation sinf;
	RegisterList registers;
	db.load_device(1, sinf, registers);
	cout << sinf.type << " ," << sinf.build_date << " ," << sinf.firmware << " ," << sinf.register_date << endl;
	for (auto& reg : registers) {
		auto& [regID, regData] = reg;
		cout << "Reg" << regID << ": [" << regData.type << " ," << regData.access << " ," << regData.value << "]" << endl;
	}
	printf("Orhan started on port %d\r\n", port);
	while (1) {

	}	

	return 1;
}
