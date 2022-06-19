#include "Location.hpp"

ft::Location::Location(const std::string &currenDir)
	: _root(currenDir),
	_return(""),
	_autoindex(true),
	_uploadPermission(false),
	_fastcgiPass(""),
	_clientMaxBodySize(DEFAULT_MAX_BODY_SIZE)
{}

ft::Location::~Location() {}

ft::Location::Location(const Location & other)
	: _root(other._root),
	_return(other._return),
	_methods(other._methods),
	_autoindex(other._autoindex),
	_uploadPermission(other._uploadPermission),
	_index(other._index),
	_fastcgiPass(other._fastcgiPass),
	_clientMaxBodySize(other._clientMaxBodySize)
{}

ft::Location&	ft::Location::operator=(const Location & other) {
	if (this != &other) {
		_root = other._root;
		_return = other._return;
		_methods = other._methods;
		_autoindex = other._autoindex;
		_uploadPermission = other._uploadPermission;
		_index = other._index;
		_fastcgiPass = other._fastcgiPass;
		_clientMaxBodySize = other._clientMaxBodySize;
	}
	return *this;
}

void	ft::Location::parseLocation(std::vector<std::string>::iterator &it,
						std::vector<std::string>::iterator &end,
						const std::string &curDir) {
	_root = curDir;
	std::string	path = *it;
	goForwardToken(it, end, 1, "{");

	while (++it != end && *it != "}") {
		if (*it == "root") {
			setRoot(it, end);
		} else if (*it == "methods") {
			setMethods(it, end);
		} else if (*it == "autoindex") {
			setAutoindex(it, end);
		} else if (*it == "upload_permission") {
			setUploadPermission(it, end);
		} else if (*it == "return") {
			setReturn(it, end);
		} else if (*it == "index") {
			setIndex(it, end);
		} else if (*it == "client_max_body_size") {
			setClientMaxBodySize(it, end);
		} if (!path.compare(0, 2, "*.") && *it == "fastcgi_pass") {
			setFastcgiPass(it, end);
		}
	}
	if (it == end)
		ft::errorExit("Invalid config file. Missing }");
}

void	ft::Location::setReturn(std::vector<std::string>::iterator &it,
			std::vector<std::string>::iterator &end)
{
	ft::goForwardToken(it, end, 1);
	_return = *it;
	ft::goForwardToken(it, end, 1, ";");	
}

void	ft::Location::setRoot(std::vector<std::string>::iterator &it,
			std::vector<std::string>::iterator &end)
{
	ft::goForwardToken(it, end, 1);
	validateDirectoryPath(*it);
	_root = *it;
	ft::goForwardToken(it, end, 1, ";");
}

void	ft::Location::setMethods(std::vector<std::string>::iterator &it,
			std::vector<std::string>::iterator &end)
{
	ft::goForwardToken(it, end, 1);
	_methods.assign(NUMBER_OF_METHODS, false);
	for (int i = 0; *it != ";"; ++i) {
		if (i == NUMBER_OF_METHODS) {
			errorExit("Invalid method in config file");
		} else if (*it == "GET" && !_methods[GET]) {
			_methods[GET] = true;
		} else if (*it == "POST" && !_methods[POST]) {
			_methods[POST] = true;
		} else if (*it == "DELETE" && !_methods[DELETE]) {
			_methods[DELETE];
		} else if (*it == "PUT" && !_methods[PUT]) {
			_methods[PUT];
		}
		goForwardToken(it, end, 1);
	}
}

void	ft::Location::setAutoindex(std::vector<std::string>::iterator &it,
			std::vector<std::string>::iterator &end)
{
	ft::goForwardToken(it, end, 1);
	if (*it == "on")
		_autoindex = true;
	else if (*it == "off")
		_autoindex = false;
	else
		errorExit("Invalid autoindex value in config file");
}

void	ft::Location::setUploadPermission(std::vector<std::string>::iterator &it,
			std::vector<std::string>::iterator &end)
{
	ft::goForwardToken(it, end, 1);
	if (*it == "on")
		_uploadPermission = true;
	else if (*it == "off")
		_uploadPermission = false;
	else
		errorExit("Invalid upload permission value in config file");
}

void	ft::Location::setIndex(std::vector<std::string>::iterator &it,
			std::vector<std::string>::iterator &end)
{
	ft::goForwardToken(it, end, 1);
	for (int i = 0; *it != ";"; ++i) {
		if (i == 10) {
			errorExit("Too many index values in config file");
		}
		_index.push_back(*it);
		goForwardToken(it, end, 1);
	}
}

void	ft::Location::setFastcgiPass(std::vector<std::string>::iterator &it,
			std::vector<std::string>::iterator &end)
{
	ft::goForwardToken(it, end, 1);
	_fastcgiPass = *it;
	ft::goForwardToken(it, end, 1, ";");
}


void	ft::Location::setClientMaxBodySize(std::vector<std::string>::iterator &it,
			std::vector<std::string>::iterator &end)
{
	ft::goForwardToken(it, end, 1);
	if (!ft::isNumber(*it))
		ft::errorExit("Invalid format of client_max_body_size in config file");
	else if (it->length() > 3
		|| (_clientMaxBodySize = static_cast<unsigned long>(atoi(it->c_str())) * 1048576)
			> MAXIMUM_MAX_BODY_SIZE)
	{
		ft::errorExit("client_max_body_size in config file exceeds the maximum value");
	}
	ft::goForwardToken(it, end, 1, ";");
}

const std::string				&ft::Location::getRoot() const {
	return _root;
}

const std::string				&ft::Location::getReturn() const {
	return _return;
}

const std::vector<bool>			&ft::Location::getMethods() const {
	return _methods;
}

bool							ft::Location::getAutoindex() const {
	return _autoindex;
}

bool							ft::Location::getUploadPermission() const {
	return _uploadPermission;
}

const std::vector<std::string> 	&ft::Location::getIndex() const {
	return _index;
}

const std::string				&ft::Location::getFastcgiPass() const {
	return _fastcgiPass;
}

unsigned long					ft::Location::getClientMaxBodySize() const {
	return _clientMaxBodySize;
}
