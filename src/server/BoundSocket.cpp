#include "BoundSocket.hpp"

ft::BoundSocket::BoundSocket() {}

ft::BoundSocket::BoundSocket(in_addr_t addr, int port) {
	setServerAddressStructure(addr, port);
	bindAddressToSocket();
}

ft::BoundSocket::~BoundSocket() {}

ft::BoundSocket::BoundSocket(const ft::BoundSocket &other)
	: _servAddr(other._servAddr)
{}

ft::BoundSocket &ft::BoundSocket::operator=(const ft::BoundSocket &other) {
	if (this != &other) {
		_servAddr = other._servAddr;
	}
	return *this;
}

void	ft::BoundSocket::bindAddressToSocket() {
	if (bind(_socket, (struct sockaddr *)&_servAddr, sizeof(_servAddr)) < 0)
		ft::systemErrorExit("bind error");
}

void ft::BoundSocket::setServerAddressStructure(in_addr_t addr, int port) {
   	std::fill_n((char *)&_servAddr, sizeof(_servAddr), '\0');
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_addr.s_addr = addr;
	_servAddr.sin_port = htons(port);
}
