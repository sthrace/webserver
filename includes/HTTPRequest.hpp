#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include <string>
# include <vector>
# include <map>
# include "HTTPConstants.hpp"

namespace ft
{
	class HTTPRequest {
		public:
			HTTPRequest();
			HTTPRequest(const HTTPRequest &other);
			HTTPRequest(const char* str);
			~HTTPRequest();

			HTTPRequest &operator=(const HTTPRequest &other);

			void				setDefault();
			void 				setMethod(std::string requestMethod);
			std::string			getMethodName() const;
			void				setHTTPVersion(std::string HTTPVersion);
			std::string			getHTTPVersion() const;
			int					setBadRequest(int status);
			void				setStatus(int status);
			int					getStatus() const;
			void				setURI(const std::string& requestURI);
			const std::string&	getProtocol() const;
			bool				setPort(const std::string& port);
			int					getPort() const;
			const std::string&	getServerName() const;
			const std::string&	getRelativePath() const;
			const std::string&	getQueryString() const;
			std::string			getFullURL() const;
			int					isParsed() const;
			const std::map<std::string, std::string>&	getHeaders() const;
   			const std::string&	getBody() const;
			unsigned long		getContentLength() const;
			void				parse(const std::string& buf);

		protected:
			enum e_versions
			{
				HTTP_0_9,
    			HTTP_1_0,
    			HTTP_1_1,
    			HTTP_2_0,
				NUMBER_OF_VERSIONS
			};

			int									_requestMethod;
			std::string							_requestURI;
			int									_HTTPVersion;
			std::string							_protocol;
			std::string							_serverName;
			std::string							_relativePath;
			std::string							_queryString;
			int									_port;
			std::map<std::string, std::string>	_headers;
			std::string							_body;
			int									_parsed;
			int									_status;
			bool								_chunked;
			unsigned long						_contentLength;
			unsigned long						_clientMaxBodySize;
			std::string							_buffer;
			std::string::size_type				_pos;

			void				parseRequestLine();
			void				parseHeaders();
			void				parseBody();
			void				parseChunkedBody();
			void				processHeaders();
			void				printParsedDataForTesting();
	};
}

#endif