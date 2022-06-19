#ifndef VIRTUAL_HOST_HPP
# define VIRTUAL_HOST_HPP

# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <map>
# include <vector>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include "HTTPConstants.hpp"
# include "Location.hpp"
# include "utils.hpp"

namespace ft {
	class VirtualHost
	{
	public:
		VirtualHost(const std::string &currentDir = "");
		~VirtualHost();
		VirtualHost(const VirtualHost &other);

		VirtualHost &operator=(const VirtualHost &other);

		void	setHost(std::vector<std::string>::iterator &it,
					std::vector<std::string>::iterator &end);
		void	setPort(std::vector<std::string>::iterator &it,
					std::vector<std::string>::iterator &end);
		void	setServerName(std::vector<std::string>::iterator &it,
					std::vector<std::string>::iterator &end);
		void	setRoot(std::vector<std::string>::iterator &it,
					std::vector<std::string>::iterator &end);
		void	setClientMaxBodySize(std::vector<std::string>::iterator &it,
					std::vector<std::string>::iterator &end);
		void	setErrorPage(std::vector<std::string>::iterator &it,
					std::vector<std::string>::iterator &end);
		void	setLocation(std::vector<std::string>::iterator &it,
					std::vector<std::string>::iterator &end);
		
		const std::string	&getRoot() const;
		in_addr_t	getHost() const;
		int	getPort() const;
		const std::string &getServerName() const;
		unsigned long	getClientMaxBodySize() const;
		const std::map<short, std::string> &getErrorPages() const;
		const std::map<std::string, ft::Location> &getLocations() const;

	private:
		std::string										_root;
		in_addr_t										_host;
		int												_port;
		std::string										_serverName;
		unsigned long									_clientMaxBodySize;
		std::map<short, std::string>					_errorPages;
		std::map<std::string, ft::Location>				_locations;
	};
}
#endif