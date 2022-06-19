#ifndef SOCKET_HPP
# define SOCKET_HPP

# include	<sys/types.h>	// basic system data types
# include	<sys/socket.h>	// basic socket definitions

# define	SERV_PORT	80

# include <fcntl.h>
# include <algorithm>
# include "utils.hpp"
	
namespace ft
{
	class Socket {
		public:
			Socket();
			Socket(const Socket &other);
			virtual ~Socket();

			Socket &operator=(const Socket &other);

			int	getSocket();

		protected:
			int	_socket;

			void	createSocket();
	};	
}


#endif