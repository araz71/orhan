#include <iostream>
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <errno.h>

#include "Packet.h"

using namespace std;

int main() {
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		cout << "Can not create socket."  << endl;
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9000);
	addr.sin_addr.s_addr = inet_addr("37.114.246.158");
	if (connect(socket_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
		cout << "Connection failed." << endl;
		cout << strerror(errno) << endl;
	}

	return 1;
}
