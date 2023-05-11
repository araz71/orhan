#include "Server.h"
#include "Database.h"

#include <iostream>

#include <time.h>
#include <stdlib.h>

using namespace std;
using namespace orhan;

int main() {
	const uint16_t port = 9000;
	Server orhan(port);

	srand(time(NULL));

	uint32_t deviceID = 1;
	RegisterID reg10 = 10;
	RegisterID reg11 = 11;

	DeviceInformation inf = {"I/O", "2.1", "May 10 23", "May 10 23"};
	Database::add_device(deviceID, inf);
	Database::add_register(deviceID, reg10, RegisterTypes::TYPE_UINT8_T, RegisterAccess::ACCESS_READ_WRITE);
	Database::add_register(deviceID, reg11, RegisterTypes::TYPE_UINT16_T, RegisterAccess::ACCESS_READ);

	DeviceInformation sinf;
	RegisterList registers;
	Database::load_device(deviceID, sinf, registers);
	cout << sinf.type << " ," << sinf.build_date << " ," << sinf.firmware << " ," << sinf.register_date << endl;
	for (auto& reg : registers) {
		auto& regData = reg.second;
		cout << "Reg" << regData.regID << ": [" << regData.type << " ," << regData.access << " ," << regData.value << "]" << endl;
	}
	printf("Orhan started on port %d\r\n", port);
	while (1) {

	}	

	return 1;
}
