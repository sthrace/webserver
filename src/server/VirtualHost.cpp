#include "VirtualHost.hpp"

ft::VirtualHost::VirtualHost(const std::string &currentDir) 
	: _root(currentDir + '/'),
	_host(INADDR_NONE),
	_port(DEFAULT_PORT),
	_serverName(""),
	_clientMaxBodySize(DEFAULT_MAX_BODY_SIZE)
{}

ft::VirtualHost::VirtualHost(const ft::VirtualHost &other)
	: _root(other._root),
	_host(other._host),
	_port(other._port),
	_serverName(other._serverName),
	_clientMaxBodySize(other._clientMaxBodySize),
	_errorPages(other._errorPages),
	_locations(other._locations)
{}

ft::VirtualHost::~VirtualHost() {}

ft::VirtualHost &ft::VirtualHost::operator=(const ft::VirtualHost &other) {
	if (this != &other) {
		_root = other._root;
		_host = other._host;
		_port = other._port;
		_serverName = other._serverName;
		_clientMaxBodySize = other._clientMaxBodySize;
		_errorPages = other._errorPages;
		_locations = other._locations;
	}
	return *this;
}

void	ft::VirtualHost::setHost(std::vector<std::string>::iterator &it,
					std::vector<std::string>::iterator &end) {
	goForwardToken(it, end, 1);
	_host = inet_addr(it->data());
	if (_host == INADDR_NONE)
		ft::errorExit("Invalid host in config file");
	goForwardToken(it, end, 1, ";");
}

void	ft::VirtualHost::setPort(std::vector<std::string>::iterator &it,
					 std::vector<std::string>::iterator &end) {
	goForwardToken(it, end, 1);
	if (!ft::isNumber(*it) || it->length() > 6
		|| (_port = atoi(it->c_str())) < 1 || _port > 65535)
		ft::errorExit("Invalid port in config file");
	goForwardToken(it, end, 1, ";");
}

void	ft::VirtualHost::setServerName(std::vector<std::string>::iterator &it,
			std::vector<std::string>::iterator &end) {
	goForwardToken(it, end, 1);
	_serverName = *it;
	goForwardToken(it, end, 1, ";");
}

void	ft::VirtualHost::setClientMaxBodySize(std::vector<std::string>::iterator &it,
			std::vector<std::string>::iterator &end) {
	goForwardToken(it, end, 1);
	if (!ft::isNumber(*it))
		ft::errorExit("Invalid format of client_max_body_size in config file");
	else if (it->length() > 3
		|| (_clientMaxBodySize = static_cast<unsigned long>(atoi(it->c_str())) * 1048576)
			> MAXIMUM_MAX_BODY_SIZE)
	{
		ft::errorExit("client_max_body_size in config file exceeds the maximum value");
	}
	goForwardToken(it, end, 1, ";");
}

void	ft::VirtualHost::setErrorPage(std::vector<std::string>::iterator &it,
					std::vector<std::string>::iterator &end) {
	short 		error_code = 0;
	struct stat statbuf;

	goForwardToken(it, end, 1);
	if (!(ft::isNumber(*it)) || it->length() > 3
		|| (error_code = atoi(it->c_str())) < 100 || error_code > 599)
	{
		ft::errorExit("Invalid format of error code in config file");
	}
	goForwardToken(it, end, 1);
	std::string fullPath = std::string(_root + *it);
    if (stat(fullPath.data(), &statbuf) < 0)
		ft::systemErrorExit("can't open error_page " 
			+ ft::to_string(error_code) + " in config file" );
	if (!S_ISREG(statbuf.st_mode))
		ft::errorExit("error_page :" + ft::to_string(error_code) 
			+ "in config file is not a file");
	_errorPages.insert(std::make_pair(error_code, fullPath));
	goForwardToken(it, end, 1, ";");
}

void	ft::VirtualHost::setLocation(std::vector<std::string>::iterator &it,
					std::vector<std::string>::iterator &end) {
	Location location;

	goForwardToken(it, end, 1);
	std::string path = *it;
	location.parseLocation(it, end, _root);
	_locations.insert(std::make_pair(path, location));
}

const std::string	&ft::VirtualHost::getRoot() const {
	return _root;
}

in_addr_t	ft::VirtualHost::getHost() const {
	return _host;
}

int	ft::VirtualHost::getPort() const {
	return _port;
}

const std::string &ft::VirtualHost::getServerName() const {
	return _serverName;
}

unsigned long	ft::VirtualHost::getClientMaxBodySize() const {
	return _clientMaxBodySize;
}

const std::map<short, std::string> &ft::VirtualHost::getErrorPages() const {
	return _errorPages;
}

const std::map<std::string, ft::Location> &ft::VirtualHost::getLocations() const {
	return _locations;
}
