#ifndef HTTPCLIENT_HPP
# define HTTPCLIENT_HPP

# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"

namespace ft {
	class HTTPClient {
		public:
			HTTPClient(int fd);
			HTTPClient(const HTTPClient &other);

			~HTTPClient();

			HTTPClient &operator=(const HTTPClient &other);

			ft::HTTPRequest	*getHttpRequest();
			int						getFd() const;
			void					response(std::map<std::string, ft::VirtualHost>& virtualHosts);
			void					parse(const char *buf);			

		private:
			ft::HTTPRequest		*_httpRequest;
			int					_fd;
			HTTPClient();
	};
	
} 


#endif