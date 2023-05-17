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

	/// Socket file descriptor
	int server_descriptor;

	/// Keeps number of clients are online
	int number_of_clients;

	/// List of online clients with it's socket descriptor
	std::unordered_map<int, orhan::Client> clients;
	/// Mutex to lock clients list when writing or reading
	std::mutex clients_mutex;

	/// Listener thread callback
	void accepter();

	/// Client reader thread callback
	void reader();

public:
	Server(uint16_t port_number);
};

}

#endif
