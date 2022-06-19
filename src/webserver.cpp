#include "Server.hpp"

int main(int argc, char *argv[]) {
	if (argc != 2)
		ft::errorExit("Usage: webserv <configuration file>");
	ft::validateFilePath(argv[1]);
	ft::Config	config(argv[1]);
	ft::Server::getInstance(config.getVirtualHosts());
    return 0;
}
