#include "Server.h"
#include "Database.h"
#include "CLI.h"

#include <iostream>

#include <time.h>
#include <stdlib.h>

using namespace std;
using namespace orhan;

int main() {
	const uint16_t port = 9000;
	Server orhan(port);
	CLI::get_cli();

	srand(time(NULL));

	DeviceInformation inf;
	RegisterMap reg;
	Database::load_device(1, inf, reg);

	for (auto reginf : reg) {
		auto& [regID, reg_inf] = reginf;

		cout << regID << endl;
	}

	printf("Orhan started on port %d\r\n", port);
	while (1) {

	}	

	return 1;
}
