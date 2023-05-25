#ifndef CLI_H_
#define CLI_H_

#include "Utility.h"
#include "Database.h"

#include <string>
#include <exception>
#include <thread>
#include <memory>
#include <iostream>
#include <algorithm>
#include <map>
#include <utility>
#include <chrono>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>

namespace orhan {

class CLI {
public:
	using CommandCallback = std::function<void([[maybe_unused]] StringList&)>;

	CLI(const CLI&) = delete;
	CLI& operator=(const CLI&) = delete;

	const char *LOGIN_FAILED = "Wrong username or password entered\r\n";
	const char *LOGIN_OK = "Loggined\r\n";
	const char *COMMAND_NOT_FOUND = "Command not found\r\n";
	const char MESSAGES[3][20] = {
			"username > ",
			"password > ",
			"command > "
	};
	const char *LESS_ARGUMENTS = "Number of entered aruments is invalid";

	void response_to_client(const char* message) {
		send(client_socket_descriptor, message, strlen(message), 0);
	}

	bool collect(const char* data, uint16_t size) {
		command += std::string(data, size);
		if (command.find("\n") != std::string::npos) {
			command.erase(std::remove(command.begin(), command.end(), '\r'), command.end());
			command.erase(std::remove(command.begin(), command.end(), '\n'), command.end());

			return true;
		}

		return false;
	}

	void run_command(StringList& args) {
		auto selected_command = std::find_if(command_map.begin(), command_map.end(), [&args](
					std::pair<std::pair<std::string, std::string>, CommandCallback> cmd)
		{
				if (cmd.first.first == args[0])
					return true;

				return false;
		});

		if (selected_command != command_map.end())
			selected_command->second(args);
		else
			response_to_client(COMMAND_NOT_FOUND);
	}

	void run() {
		struct sockaddr_in addr;
		socklen_t addr_len = sizeof(addr);
		int state = 0;	
		
		while (1) {
			client_socket_descriptor = accept(cli_socket_descriptor, reinterpret_cast<struct sockaddr*>(&addr), &addr_len);

			std::string username;
			std::string password;
			char buffer[1024];
	
			state = 0;
			if (client_socket_descriptor > 0) {
				while (1) {
					response_to_client(MESSAGES[state]);
					int len = recv(client_socket_descriptor, buffer, 1024, 0);
					if (len > 0) {
						collect(buffer, len);

						// Handle message
						if (state == 0) {
							username = command;
							state++;
						} else if (state == 1) {
							password = command;
							if (username == "amin" /*Configurations::get_cli_username()*/ &&
									password == "aghakhani" /*Configurations::get_cli_password()*/)
							{
								response_to_client(LOGIN_OK);
							} else {
								state = 0;
								response_to_client(LOGIN_FAILED);
								close(client_socket_descriptor);
								break;
							}

							state++;
						} else {
							StringList args;
							split_strings(command, " ", args);
							run_command(args);	
						}

						command.clear();
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
	int cli_socket_descriptor;
	int client_socket_descriptor;

	std::string command;

	std::unique_ptr<std::thread> cli_thread;

	std::map<std::pair<std::string, std::string>, CommandCallback> command_map;

	CLI() {
		cli_socket_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (cli_socket_descriptor == -1)
			throw std::runtime_error("CLI : Can not create socket");

		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(1818); //htons(Configurations::get_cli_port_number());
		addr.sin_addr.s_addr = INADDR_ANY;

		if (bind(cli_socket_descriptor, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0)
			throw std::runtime_error("CLI : Can not bind socket on port");

		if (listen(cli_socket_descriptor, 1) < 0)
			throw std::runtime_error("CLI : Can not listen");

		command_map[std::make_pair("add-device", "Adds new device to database.")] = [this](StringList& args) {
			if (args.size() != 3)
				response_to_client(LESS_ARGUMENTS);
			else {
				auto now = std::chrono::system_clock::now();
				std::time_t end_time = std::chrono::system_clock::to_time_t(now);
				std::string string_time{ctime(&end_time)};

				DeviceInformation device_info = {
					.type = args[2],
					.register_date = string_time
				};

				uint32_t device_id = std::stoi(args[1]);
				if (!Database::add_device(device_id, device_info))
					response_to_client("Selected device is added before!\r\n");
				else
					response_to_client("Device added successfully\r\n");
			}
		};

		command_map[std::make_pair("remove-device", "Removes device from database")] = [](StringList& args) {
			std::cout << "you wanna remove" << std::endl;
		};

		command_map[std::make_pair("add-register", "Adds new register to device")] = [](StringList& args) {
			std::cout << "You wanna add register" << std::endl;
		};

		command_map[std::make_pair("exit", "Exits from CLI")] = [this]([[unused]] StringList& args) {
			response_to_client("ByBy\r\n");
			close(client_socket_descriptor);
		};

		command_map[std::make_pair("help", "Prints all avaliable commands")] = [this]([[unused]] StringList& args) {
			for (auto& command : command_map) {
				auto [title, description] = command.first;

				std::string help_message = "\t";
				help_message += title;
				help_message += " : ";
				help_message += description;
				help_message += "\r\n";

				response_to_client(help_message.c_str());
			}
		};

		cli_thread.reset(new std::thread(&CLI::run, this));		
	}
};

}
#endif
