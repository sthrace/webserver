#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <unistd.h>
# include <errno.h>
# include <poll.h>
# include <signal.h>
# include "ListeningSocket.hpp"
# include "Config.hpp"
# include "HTTPClient.hpp"

/* 
** POSIX requires that an #include of <poll.h> define INFTIM, but many
** systems still define it in <sys/stropts.h>.  We don't want to include
** all the STREAMS stuff if it's not needed, so we just define INFTIM here.
** This is the standard value, but there's no guarantee it is -1. 
*/

# ifndef INFTIM
#  define INFTIM	(-1)    // infinite poll timeout
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX	10240	// max open files per process
# endif

#define	MAXLINE		4096	// max text line length


/* Define for notifications*/
# define NOTIFICATIONS 1

namespace ft
{
	class Server
	{
	public:
		static Server *getInstance(const std::map<std::string, ft::VirtualHost> &virtualHosts);
	private:
		static Server *_server;

		Server();
		Server(const std::map<std::string, ft::VirtualHost> &virtualHosts);
		Server(const Server &other);
		virtual ~Server();
		
		Server &operator=(const Server &other);

		std::map<std::string, ft::VirtualHost>	_virtualHosts;
		std::map<int, ft::HTTPClient *>			_httpClients;
		std::vector<ft::ListeningSocket *>					_listeningSockets;
		struct pollfd										_client[OPEN_MAX];

		void		initializeListennersPollfd();
		void		run();
		void		checkConnectionsForData(int	maxIdx, int countReadyFd);
		void		registerSignals();
		static void	handleShutdown(int signal);
		std::string	sockNtop(const struct sockaddr *sa);
		void		setListeningSockets();
		void		freeClient(int i);
	};
} // namespace ft

#endif
