#include "Server.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

ft::Server* ft::Server::_server = (Server *)0;

ft::Server::Server(const std::map<std::string, ft::VirtualHost> &virtualHosts)
	: _virtualHosts(virtualHosts) {
	registerSignals();
	timestamp("Starting up..");
	setListeningSockets();
	run();
}

ft::Server::~Server() {
	std::vector<ListeningSocket *>::iterator it = _listeningSockets.begin();
	std::vector<ListeningSocket *>::iterator end = _listeningSockets.end();

	while (it != end) {
		delete(*it);
		++it;
	}
	delete _server;
}

ft::Server	*ft::Server::getInstance(const std::map<std::string, ft::VirtualHost> &virtualHosts) {
	if (_server == (Server *)0) {
	    _server = new Server(virtualHosts);
	}
	return _server;
}

void	ft::Server::setListeningSockets() {
	std::map<std::string, ft::VirtualHost>::iterator it = _virtualHosts.begin();
	std::map<std::string, ft::VirtualHost>::iterator end = _virtualHosts.end();

	while (it != end) {
		_listeningSockets.push_back(new ft::ListeningSocket(it->second.getHost() ,it->second.getPort()));
		++it;
	}
}

void	ft::Server::initializeListennersPollfd() {
	size_t	n = _listeningSockets.size();

	for (size_t i = 0; i < n; ++i) {
		_client[i].fd = _listeningSockets[i]->getSocket();
		_client[i].events = POLLRDNORM;
	}
	for (size_t i = n; i < OPEN_MAX; i++)
		_client[i].fd = -1;					// -1 indicates available entry
}

void	ft::Server::run() {
	int					countReadyFd;		// count of ready descriptors
	int					connfd;
	struct sockaddr_in	cliaddr;
	socklen_t			clilen;
	size_t				countListeningSockets = _listeningSockets.size();
	nfds_t				j, maxIdx = countListeningSockets - 1;		// max index in _client[] array

	initializeListennersPollfd();
	while(true) {
		if ( (countReadyFd = poll(_client, maxIdx + 1, INFTIM)) < 0)
			systemErrorExit("poll error");

		for (size_t i = 0; i < countListeningSockets && countReadyFd > 0; ++i) {
			if (_client[i].revents & POLLRDNORM) {	// new client connection
				clilen = sizeof(cliaddr);
				connfd = Accept(_listeningSockets[i]->getSocket(), (struct sockaddr *) &cliaddr, &clilen);
				_httpClients.insert(std::make_pair(connfd, new ft::HTTPClient(connfd)));
				// fcntl(connfd, F_SETFL, O_NONBLOCK);
				timestamp("New client: " + sockNtop((struct sockaddr *) &cliaddr));
				for (j = countListeningSockets; j < OPEN_MAX; ++j) {
					if (_client[j].fd < 0) {
						_client[j].fd = connfd;		// save descriptor
						break;
					}
				}
				if (j == OPEN_MAX)
					errorExit("too many clients");
				_client[j].events = POLLRDNORM;
				maxIdx = std::max(maxIdx, j);
				--countReadyFd;
			}
		}
		if (countReadyFd > 0)
			checkConnectionsForData(maxIdx, countReadyFd);
	}
}

void	ft::Server::checkConnectionsForData(int	maxIdx, int countReadyFd) {
	int		sockfd;
	ssize_t	n;
	char	buf[MAXLINE + 1];

	// check all clients for data
	for (int i = _listeningSockets.size(); i <= maxIdx && countReadyFd > 0; ++i) {
		if ( (sockfd = _client[i].fd) < 0)
			continue;
		if (_client[i].revents & (POLLRDNORM | POLLERR)) {
			--countReadyFd;
			if ( (n = recv(sockfd, buf, MAXLINE, 0)) < 0) {
				if (errno == ECONNRESET) {	// connection reset by client
					timestamp("_client[" + ft::to_string(i) +"] aborted connection");
					freeClient(i);
				} else
					systemErrorExit("read error");
			} else if (n == 0) {			// connection closed by client
				timestamp("_client[" + ft::to_string(i) +"] closed connection");
				freeClient(i);
			} else {
				buf[MAXLINE] = '\0';
				ft::HTTPClient  *httpClient = _httpClients.at(sockfd);
				httpClient->parse(buf);
				if (httpClient->getHttpRequest()->isParsed() == YES) {
					httpClient->response(_virtualHosts);

					ft::HTTPRequest *httpRequest = httpClient->getHttpRequest();
					std::map<std::string, std::string>::const_iterator it = httpRequest->getHeaders().find("connection");
					std::string connection;
					it != httpRequest->getHeaders().end() ? connection = it->second : connection = "";
					
					if (httpRequest->getStatus() != HTTP_OK) {
						timestamp("closed connection with _client[" 
							+ ft::to_string(i) +"] on error "
							+ ft::to_string(httpRequest->getStatus()));
						freeClient(i);
					} else if (httpRequest->getHTTPVersion() == "HTTP/0.9"
							|| (httpRequest->getHTTPVersion() == "HTTP_1_0:"
							&& connection != "keep-alive")
							|| (httpRequest->getHTTPVersion() == "HTTP_1_1"
							&& connection == "close")
							) 
					{
						timestamp("server closed connection with _client[" + ft::to_string(i) +"] ");
						freeClient(i);
					} else {
						httpRequest->setDefault();
					}
				}
			}
		}
	}
}

void ft::Server::freeClient(int i) {
	std::map<int, ft::HTTPClient *>::iterator it
		= _httpClients.find(_client[i].fd);
	
	Close(_client[i].fd);
	if (it != _httpClients.end()) {
		delete _httpClients.at(_client[i].fd);
		_httpClients.erase(_client[i].fd);
	}
	_client[i].fd = -1;
}

void ft::Server::handleShutdown(int signal) {
	timestamp("Shutting down on signal " + ft::to_string(signal));
    exit(signal);
}

void	ft::Server::registerSignals() {
	signal(SIGTERM, ft::Server::handleShutdown);   // call handleShutdown when killed
   	signal(SIGINT, ft::Server::handleShutdown);   // call handleShutdown when interrupted
}

/*
* The function sockNtop takes a pointer to a socket address structure,
* looks inside the structure, return the string presentation
* of socket address (ip_address:port)
*/
std::string	ft::Server::sockNtop(const struct sockaddr *sa) {
    char		str[INET6_ADDRSTRLEN];		// IP6 is larger then IP4
	std::string	socket;

	switch (sa->sa_family) {
		case AF_INET: {
			struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

			if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
				socket = "not_nown_IP";
			else
				socket = std::string(str) + ':' + ft::to_string(ntohs(sin->sin_port));
			break;
		}
		case AF_INET6: {
			struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

			if (inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) == NULL)
				socket = "not_nown_IP";
			else 
				socket = '[' + std::string(str) + "]:" + ft::to_string(ntohs(sin6->sin6_port));
		}
	}
	return socket;
}
