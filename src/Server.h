#ifndef _SERVER_H_
#define _SERVER_H_

#include "Client.h"

#include <thread>
#include <memory>
#include <vector>
#include <mutex>
#include <map>

#include <stdint.h>

namespace orhan
{

class Server {
 private:
	std::unique_ptr<std::thread> accepter_thread;
	std::unique_ptr<std::thread> reader_thread;
	std::unique_ptr<std::thread> writer_thread;

	int server_descriptor;

	int number_of_clients;

        std::unordered_map<int, orhan::Client> clients;
	std::mutex clients_mutex;

	void accepter();
	void reader();
 public:
	Server(uint16_t port_number);
};

}

#endif
