#include "ListeningSocket.hpp"

ft::ListeningSocket::ListeningSocket(in_addr_t addr, int port)
	: BoundSocket(addr, port)
{
	startListening();
}

ft::ListeningSocket::ListeningSocket(const ListeningSocket &other)
	: _listeningSocket(other._listeningSocket)
{}

ft::ListeningSocket::~ListeningSocket(){}

ft::ListeningSocket &ft::ListeningSocket::operator=(const ft::ListeningSocket& other) {
	if (this != &other) {
		_listeningSocket = other._listeningSocket;
	}
	return *this;
}

void	ft::ListeningSocket::startListening() {
	char	*ptr;
	int		backlog;

	/* Can override 2nd argument with environment variable */
	if ( (ptr = std::getenv("LISTENQ")) != NULL)
		backlog = std::atoi(ptr);
	else
		backlog = SOMAXCONN;

	if (listen(_socket, backlog) < 0)
		ft::systemErrorExit("listen error");
}

