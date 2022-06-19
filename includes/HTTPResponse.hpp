#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include <fstream>
# include <map>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include "HTTPRequest.hpp"
# include "VirtualHost.hpp"

namespace ft {
	class HTTPResponse {
		public:
			HTTPResponse(HTTPRequest* httpRequest, std::map<std::string,ft::VirtualHost> &_virtualHosts);
			~HTTPResponse();

			HTTPResponse &operator=(const HTTPResponse &other);
			const std::string	getResponse();
		protected:
			HTTPRequest* 							_httpRequest;
			std::map<std::string,ft::VirtualHost>	&_virtualHosts;
			std::string								_response;
			short									_redirect;
			std::map<std::string, std::string>		_headers;
			std::string 							_res;
			std::string 							_res_path;
			std::string 							_location;
			std::vector<std::string> 				_index;
			bool 									_autoindex;
			std::string 							_fastCGI;
			
			HTTPResponse();
			HTTPResponse(const HTTPResponse &other);
			std::string GetResponse(size_t code, std::string content);
			void 		parseVHOST(void);
			std::string getResource(std::string rel_path);
			std::string	Get();
			std::string	Post();
			std::string	Delete();
			std::string 	Error(size_t code);
			std::string Autoindex(void);
			const std::string getMIME(std::string file) const;
			const std::string getStatus(size_t code) const;
			bool validMethod(std::string &method);
			bool 		isCGIRequest(void);
			std::string execCGI();
	};
	
} // namespace ft


#endif