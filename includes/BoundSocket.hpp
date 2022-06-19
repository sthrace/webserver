#ifndef BOUND_SOCKET_HPP
# define BOUND_SOCKET_HPP

# include	<netinet/in.h>	// sockaddr_in{} and other Internet defns
# include	<arpa/inet.h>	// inet(3) functions
# include	"Socket.hpp"

namespace ft
{
	class BoundSocket : public Socket
	{
		public:
			BoundSocket();
			BoundSocket(in_addr_t addr, int port);
			BoundSocket(const BoundSocket& other);
			virtual ~BoundSocket();

			BoundSocket &operator=(const BoundSocket& other);
			
		private:

			// _servAddr is a structure specifies a transport address
			// and port for the AF_INET address family
			struct sockaddr_in	_servAddr;
			
			void	setServerAddressStructure(in_addr_t addr, int port);
			void	bindAddressToSocket();
	};
		
}


#endif