#include <string>
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "utils.hpp"

ft::HTTPRequest::HTTPRequest()
	: _requestMethod(GET),
	_requestURI(""),
	_HTTPVersion(HTTP_1_1),
	_protocol("http"),
	_serverName(""),
	_relativePath(""),
	_queryString(""),
	_port(DEFAULT_PORT), 
	_headers(),
	_body(""),
	_parsed(NO),
	_status(HTTP_OK),
	_chunked(false),
	_contentLength(0),
	_clientMaxBodySize(DEFAULT_MAX_BODY_SIZE),
	_buffer(""),
	_pos(0)
{}

ft::HTTPRequest::HTTPRequest(const ft::HTTPRequest &other)
	: _requestMethod(other._requestMethod),
	_requestURI(other._requestURI),
	_HTTPVersion(other._HTTPVersion),
	_protocol(other._protocol),
	_serverName(other._serverName),
	_relativePath(other._relativePath),
	_queryString(other._queryString),
	_port(other._port),
	_headers(other._headers),
	_body(other._body),
	_parsed(other._parsed),
	_status(other._status),
	_chunked(other._chunked),
	_contentLength(other._contentLength),
	_clientMaxBodySize(other._clientMaxBodySize),
	_buffer(other._buffer),
	_pos(other._pos)
{}

ft::HTTPRequest::~HTTPRequest() {}

ft::HTTPRequest &ft::HTTPRequest::operator=(const ft::HTTPRequest &other) {
	if (this != &other) {
		_requestMethod = other._requestMethod;
		_requestURI = other._requestURI;
		_HTTPVersion = other._HTTPVersion;
		_protocol = other._protocol;
		_serverName = other._serverName;
		_relativePath = other._relativePath;
		_queryString = other._queryString;
		_port = other._port;
		_headers = other._headers;
		_body = other._body;
		_parsed = other._parsed;
		_status = other._status;
		_chunked = other._chunked;
		_contentLength = other._contentLength;
		_clientMaxBodySize = other._clientMaxBodySize;
		_buffer	= other._buffer;
		_pos = other._pos;
	}
	return *this;
}

void ft::HTTPRequest::setDefault() {
	_requestMethod = GET;
	_requestURI = "";
	_HTTPVersion = HTTP_1_1;
	_protocol = "http",
	_serverName = "",
	_relativePath = "",
	_queryString = "",
	_port = DEFAULT_PORT; 
	_headers.clear();
	_body = "";
	_parsed = NO,
	_status = HTTP_OK;
	_chunked = false;
	_contentLength = 0;
	_clientMaxBodySize = DEFAULT_MAX_BODY_SIZE;
	_buffer = "";
	_pos = 0;
}

std::string	ft::HTTPRequest::getMethodName() const {
	switch (_requestMethod)
	{
	case GET:
		return "GET";
	case POST:
		return "POST";
	case DELETE:
		return "DELETE";
	case PUT:
		return "PUT";
	default:
		return "";
	}
}

std::string ft::HTTPRequest::getHTTPVersion() const {
	switch (_HTTPVersion)
	{
		case HTTP_0_9:
			return "HTTP/0.9";
		case HTTP_1_0:
			return "HTTP/1.0";
		case HTTP_1_1:
			return "HTTP/1.1";
		case HTTP_2_0:
			return "HTTP/2.0";
		default:
			return "";
	}
}

void ft::HTTPRequest::setMethod(std::string requestMethod) {
	ft::toUpperString(requestMethod);
	for(_requestMethod = 0; _requestMethod < NUMBER_OF_METHODS; _requestMethod++) {
		if(requestMethod.compare(getMethodName()) == 0)
			return;
	}
	throw HTTP_METHOD_NOT_ALLOWED;
}

void ft::HTTPRequest::setHTTPVersion(std::string HTTPVersion) {
	ft::toUpperString(HTTPVersion);
	for(_HTTPVersion = 0; _HTTPVersion < NUMBER_OF_VERSIONS; _HTTPVersion++) {
		if(HTTPVersion.compare(getHTTPVersion()) == 0)
			return;
	}
	throw HTTP_VERSION_NOT_SUPPORTED;
}

void	ft::HTTPRequest::setStatus(int status) {
	_status = status;
}

int 	ft::HTTPRequest::getStatus() const {
	return _status;
}

int	ft::HTTPRequest::setBadRequest(int status) {
	setStatus(status);
	_parsed = YES;
	return status;
}

bool ft::HTTPRequest::setPort(const std::string& port) {
	return ft::isNumber(port) && port.length() < 6 
		&& (_port = atoi(port.c_str())) > -1 && _port < 65536;
}

int ft::HTTPRequest::getPort() const {
	return _port;
}

void	ft::HTTPRequest::setURI(const std::string& requestURI) {
	std::string::size_type	pos;
	
	if(requestURI.size() < 1)
		throw HTTP_BAD_REQUEST;
	if (requestURI[0] == '/') {									// relative path
		_relativePath = requestURI;
	} else if (requestURI.find("://") == std::string::npos) {
		throw HTTP_BAD_REQUEST;
	} else {													// absolute path
		_protocol = ft::getWithoutExtension(requestURI, "://");
		if (_protocol.compare(PROTOCOL))
			throw HTTP_BAD_REQUEST;
		std::string	tmpURI = getExtension(requestURI, "://");
		pos = tmpURI.find(":");
		if (pos != std::string::npos) {
			_serverName = ft::getWithoutExtension(tmpURI, "/");
			_relativePath = ft::getExtension(tmpURI, "/");
			_port = DEFAULT_PORT;
		} else {
			_serverName = ft::getWithoutExtension(tmpURI, ":");
			std::string::size_type end = tmpURI.find("/");			
			if (!setPort(tmpURI.substr(pos + 1, end - pos - 1)))
				throw HTTP_BAD_REQUEST;
			_relativePath = tmpURI.substr(end);
		}
	}
	if (_relativePath.find("?") != std::string::npos) {
		_queryString = ft::getExtension(_relativePath, "?");
		_relativePath = ft::getWithoutExtension(_relativePath, "?");
	}
}

const std::string&	ft::HTTPRequest::getProtocol() const {
	return _protocol;
}


const std::string&	ft::HTTPRequest::getServerName() const {
	return _serverName;
}

const std::string&	ft::HTTPRequest::getRelativePath() const {
	return _relativePath;
}

const std::string&	ft::HTTPRequest::getQueryString() const {
    return _queryString;
}

std::string ft::HTTPRequest::getFullURL() const {
	return _queryString.length()
		? _protocol + "://" + _serverName + ':' + ft::to_string(_port) + _relativePath + '?' + _queryString
		: _protocol + "://" + _serverName + ':' + ft::to_string(_port) + _relativePath;
}

int	ft::HTTPRequest::isParsed() const {
    return _parsed;
}

const std::map<std::string, std::string>&	ft::HTTPRequest::getHeaders() const {
    return _headers;
}

const std::string&	ft::HTTPRequest::getBody() const {
	return _body;
}

unsigned long	ft::HTTPRequest::getContentLength() const {
	return _contentLength;
}

void	ft::HTTPRequest::parseRequestLine() {
	std::string::size_type	tmpPos = _pos;
	std::string 			requestLine;

	if (ft::parseToken(_buffer, CRLF, tmpPos, requestLine,
						true, true, MAX_REQUEST_LINE_LENGTH))
	{
		std::vector<std::string> requestLines = ft::split(requestLine);

		if (requestLines.size() < 3)
			throw HTTP_BAD_REQUEST;
		setHTTPVersion(requestLines[2]);
		setMethod(requestLines[0]);
		if (requestLines[1].length() > MAX_URI_LENGTH)
			throw HTTP_URI_TOO_LONG;
		ft::toLowerString(requestLines[1]);
		setURI(requestLines[1]);
		_pos = tmpPos;
		_parsed |= REQUEST_LINE;
	} else if (requestLine.length() > MAX_REQUEST_LINE_LENGTH) {
		throw HTTP_BAD_REQUEST;
	}
}

void	ft::HTTPRequest::parseHeaders() {
	std::string::size_type	tmpPos = _pos;
	std::string 			headers;
	std::string headerName, headerValue;

	if (!_buffer.compare(_pos, strlen(CRLF), CRLF)) {
		++_pos;
		_parsed |= HEADERS;
		return;
	}
	if (ft::parseToken(_buffer, CRLF_CRLF, tmpPos, headers, true, true, MAX_HEADERS_LENGTH)) {
		std::vector<std::string> headerLines = ft::split(headers, CRLF);

		if (headerLines.size() > MAX_HEADER_FIELDS)
			throw HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE;

		for (size_t i = 0; i < headerLines.size(); ++i) {
			tmpPos = 0;
			if (!ft::parseToken(headerLines[i], ":", tmpPos, headerName, true, true, MAX_HEADER_NAME_LENGTH))
				throw HTTP_BAD_REQUEST;
			if (!ft::parseToken(headerLines[i], CRLF, tmpPos, headerValue, true, false, MAX_HEADER_VALUE_LENGTH))
				throw HTTP_BAD_REQUEST;
			ft::toLowerString(headerName);
			_headers.insert(std::make_pair(headerName, headerValue));
		}
		_parsed |= HEADERS;
	} else if (headers.length() > MAX_HEADERS_LENGTH) {
		throw HTTP_BAD_REQUEST;
	}
}

void	ft::HTTPRequest::processHeaders() {
	std::map<std::string, std::string>::const_iterator it;
	std::string::size_type	pos = 0;

	_parsed |= PROCESSED_HEADERS;
	it = _headers.find("host");
	if (_serverName.empty() && it != _headers.end()) {
		ft::parseToken(it->second, ":", pos, _serverName, true);
		std::string strPort;
		ft::parseToken(it->second, CRLF, pos, strPort, true);
		if (!strPort.empty() && !setPort(strPort))
			throw HTTP_BAD_REQUEST;
	} else if (_serverName.empty()) {							// server name not found
		throw HTTP_BAD_REQUEST;
	} else if (it == _headers.end()) {
		_headers.insert(std::pair<std::string, std::string>("host",
							_serverName + ":" + ft::to_string(_port)));
	}

 	it = _headers.find("transfer-encoding");
 	if (it != _headers.end()) {
		_chunked = !it->second.compare("chunked");
	} else {
		it = _headers.find("content-length");
		if (it != _headers.end()) {
			_contentLength = std::strtoul(it->second.c_str(), NULL, 10);
			if (errno == ERANGE || _contentLength > _clientMaxBodySize)
            	throw HTTP_PAYLOAD_TOO_LARGE;
		} else if (_requestMethod == POST || _requestMethod == PUT) {
			throw HTTP_LENGTH_REQUIRED;
		}
		_chunked = false;
	}

	if (_contentLength == 0 && !_chunked) {
		_parsed = YES;
		_body.clear();
	}
}

void	ft::HTTPRequest::parseBody() {
	if (!_chunked) {
		std::string::size_type	tmpPos = _pos;
		
		if (ft::parseToken(_buffer, CRLF, tmpPos, _body,
						true, true, _contentLength))
		{
			_parsed = YES;
			_pos = tmpPos;
		} else if (_body.length() > _contentLength) {
			throw HTTP_PAYLOAD_TOO_LARGE;
		}
	} else {
		parseChunkedBody();
	}
}

void	ft::HTTPRequest::parseChunkedBody() {
	std::string::size_type	tmpPos = _pos;
	std::string 			body;

	if (ft::parseToken(_buffer, "0\r\n", tmpPos, body, true, true, _clientMaxBodySize)) {
		std::string::size_type	bodyLen = body.length();
		std::string::size_type	chunkSize = 0;
		std::string				chunkLen;

		_pos = tmpPos;
		tmpPos = 0;
		while (tmpPos < bodyLen) {
			if (body.find(CRLF, tmpPos) == std::string::npos)
				throw HTTP_BAD_REQUEST;
			while (std::isspace(body[tmpPos]))
				++tmpPos;
			while (std::isxdigit(body[tmpPos]))
				chunkLen += body[tmpPos++];
			if (body[tmpPos] != ';' && body.find(CRLF, tmpPos) != tmpPos)
				throw HTTP_BAD_REQUEST;
			chunkSize = std::strtoul(chunkLen.data(), NULL, 16);
			chunkLen.clear();
			tmpPos = body.find(CRLF, tmpPos) + 2;
			if (tmpPos < bodyLen) {
				size_t end = chunkSize > bodyLen - tmpPos ? bodyLen - tmpPos : chunkSize;
				_body.append(body, tmpPos, end); 
				tmpPos += end;
				chunkSize -= end;
			}
		}
		_parsed = YES;
		_headers.erase(_headers.find("transfer-encoding"));
		_headers["content-length"] = ft::to_string(_body.size());
		_contentLength = _body.length();
	} else if (body.length() > _clientMaxBodySize) {
		throw HTTP_BAD_REQUEST;
	}

}

void	ft::HTTPRequest::parse(const std::string& buf) {
	_buffer.append(buf);
	try {
		if (!(_parsed & REQUEST_LINE))
			parseRequestLine();
		if ((_parsed & REQUEST_LINE) && !(_parsed & HEADERS) && _pos < _buffer.length())
			parseHeaders();
		if ((_parsed & HEADERS) && !(_parsed & PROCESSED_HEADERS) && _pos < _buffer.length())
			processHeaders();
		if ((_parsed & PROCESSED_HEADERS) && !(_parsed & BODY) && _pos < _buffer.length())
        	parseBody();
	} catch (int statusCode) {
		setBadRequest(statusCode);
	}

	// printParsedDataForTesting();
}

/*
**	This function is for testing parsed data from HTTP requests.
*/
void	ft::HTTPRequest::printParsedDataForTesting() {
	std::cout << "\n\n" << GREEN_COLOR << "PARSED DATA:\n" << "method: " << getMethodName()
				<< ", target: " << _requestURI << ", HTTP version " << getHTTPVersion()
				<< "\nPARSED URI:\n" << "protocol: " << getProtocol()
				<< ", host: " << getServerName() << ", port: " << getPort()
				<< ", relative path: " << getRelativePath() 
				<< ", query string: " << getQueryString() 
				<< "\nfull uri: " << getFullURL()
				<< "\ncontent-length: " << getContentLength()
				<< "\nChunked: " << _chunked
				<< "\nParsed: " << isParsed() << ", response status: " << getStatus() << RESET_COLOR << std::endl;
	
	std::map<std::string, std::string> tmpHeaders = getHeaders();
	std::map<std::string, std::string>::const_iterator it = tmpHeaders.begin();
	std::cout << "HEADERS:\n";
	for (; it != tmpHeaders.end(); ++it) {
		std::cout << it->first << ": " << it->second << '\n';
	}
	std::cout << "BODY:\n";
		std::cout << _body << std::endl;
}
