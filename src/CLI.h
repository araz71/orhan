#ifndef CLI_H_
#define CLI_H_

#include "Utility.h"

#include <string>
#include <exception>
#include <thread>
#include <memory>
#include <iostream>
#include <algorithm>
#include <map>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>

namespace orhan {

class CLI {
public:

	CLI(const CLI&) = delete;
	CLI& operator=(const CLI&) = delete;

	void response_to_client(const char* message) {
		send(client_socket_descriptor, message, strlen(message), 0);
	}

	void run() {
		struct sockaddr_in addr;
		socklen_t addr_len = sizeof(addr);
		int state = 0;
		const char messages[3][20] = {
			"username > ",
			"password > ",
			"command > "
		};

		const char *login_failed = "Wrong username or password entered\r\n";
		const char *login_ok = "Loggined\r\n";
		const char *command_not_found = "Command not found\r\n";

		std::map<std::string, std::function<void([[maybe_unused]] StringList& args)>> command_list;
		command_list["add-device"] = [](StringList& args) {
			std::cout << "You wanna add" << std::endl;
		};
		command_list["remove-device"] = [](StringList& args) {
			std::cout << "you wanna remove" << std::endl;
		};
		command_list["add-register"] = [](StringList& args) {
			std::cout << "You wanna add register" << std::endl;
		};

		command_list["exit"] = [&state]([[unused]] StringList& args) {
			state = 100;
		};

		command_list["help"] = [&command_list, this]([[unused]] StringList& args) {
			for (auto& command : command_list) {
				std::string help_message = "\t";
				help_message += command.first;
				help_message += " : ";
				help_message += "\r\n";

				response_to_client(help_message.c_str());
			}
		};

		
		while (1) {
			client_socket_descriptor = accept(socket_descriptor, reinterpret_cast<struct sockaddr*>(&addr), &addr_len);

			std::string command;	
			std::string username;
			std::string password;
			char buffer[1024];
	
			state = 0;
			if (client_socket_descriptor > 0) {
				while (1) {
					response_to_client(messages[state]);
					int len = recv(client_socket_descriptor, buffer, 1024, 0);
					if (len > 0) {
						command += std::string(buffer, len);
						if (command.find("\n") != std::string::npos) {
							command.erase(std::remove(command.begin(), command.end(), '\r'), command.end());
							command.erase(std::remove(command.begin(), command.end(), '\n'), command.end());

							// Handle message
							if (state == 0) {
								username = command;
								state++;
							} else if (state == 1) {
								password = command;
								if (username == "amin" /*Configurations::get_cli_username()*/ &&
										password == "aghakhani" /*Configurations::get_cli_password()*/)
								{
									response_to_client(login_ok);
								} else {
									state = 0;
									response_to_client(login_failed);
									close(client_socket_descriptor);
									break;
								}

								state++;
							} else {
								StringList args;
								split_strings(command, " ", args);
								auto selected_command = command_list.find(args[0]);
								if (selected_command != command_list.end())
									selected_command->second(args);
								else
									response_to_client(command_not_found);
	
								if (state == 100) {
									state = 0;
									response_to_client("ByBy\r\n");
									close(client_socket_descriptor);
									break;
								}
							}
							
							command.clear();
						}
					}
				}
			}
		}
	}

	static CLI& get_cli() {
		static CLI command_line_interface;
		return command_line_interface;
	}
private:
	int socket_descriptor;
	int client_socket_descriptor;

	std::unique_ptr<std::thread> cli_thread;

	CLI() {
		socket_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socket_descriptor == -1)
			throw std::runtime_error("CLI : Can not create socket");

		/*struct ifreq ifr;
		memset(&ifr, 0, sizeof(ifr));
		strncpy(ifr.ifr_name, "lo", IFNAMSIZ - 1);
		if (setsockopt(socket_descriptor, SOL_SOCKET, SO_BINDTODEVICE, reinterpret_cast<void*>(&ifr), sizeof(ifr)) < 0)
			throw std::runtime_error("CLI : Binding on lo failed");
*/
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(1818); //htons(Configurations::get_cli_port_number());
		addr.sin_addr.s_addr = INADDR_ANY;

		if (bind(socket_descriptor, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0)
			throw std::runtime_error("CLI : Can not bind socket on port");

		if (listen(socket_descriptor, 5) < 0)
			throw std::runtime_error("CLI : Can not listen");

		cli_thread.reset(new std::thread(&CLI::run, this));		
	}
};

}
#endif
