#include "Server.h"

#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <chrono>
#include <mutex>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace orhan;

Server::Server(const uint16_t port_number) {
	server_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (server_descriptor < 0)
		throw runtime_error("Can not create socket\r\n");

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port_number);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_descriptor, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
		throw runtime_error("Can not bind on port " + to_string(port_number) + "\r\n");

	listen(server_descriptor, 1);

	accepter_thread.reset(new thread(&Server::accepter, this));
	reader_thread.reset(new thread(&Server::reader, this));
}

void Server::accepter() {
	struct sockaddr_in client_addr;
	socklen_t address_length = sizeof(client_addr);

	while (1) {
		const int new_client_descriptor = accept(server_descriptor, (struct sockaddr*)&client_addr,
				&address_length);

		if (new_client_descriptor != -1) {
			clients_mutex.lock();
			clients[new_client_descriptor] = Client(new_client_descriptor, client_addr.sin_addr.s_addr);
			clients_mutex.unlock();
		}
	}
}

void Server::reader() {
	uint8_t buffer[512];

	while(1) {
		this_thread::sleep_for(chrono::milliseconds(10));
		clients_mutex.lock();
		for (auto& client : clients) {
			auto& [socket_descriptor, client_object] = client;

			const int size = recv(socket_descriptor, buffer, sizeof(buffer), MSG_DONTWAIT);
			if (size > 0) {
				string response;
				if (!client_object.handle_packet(buffer, size, response)) {
					cout << "Can not atach packet" << endl;

				} else if (response.size() > 0) {
					const size_t write_len = send(socket_descriptor, response.c_str(),
							response.length(), MSG_DONTWAIT);
					client_object.update_communication();
				}
			}
		}
		clients_mutex.unlock();
	}
}
