#include "Client.h"

#include <thread>
#include <memory>
#include <vector>
#include <mutex>

#include <stdint.h>

class Server {
 private:
	std::unique_ptr<std::thread> accepter_thread;
	std::unique_ptr<std::thread> reader_thread;

	int server_descriptor;

	int number_of_clients;

	std::vector<Client> clients;
	std::mutex clients_mutex;

	void accepter();
	void reader();
 public:
	Server(uint16_t port_number);
};
