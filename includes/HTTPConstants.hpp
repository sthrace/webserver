#ifndef HTTP_RESPONSE_STATUS_CODES
# define HTTP_RESPONSE_STATUS_CODES

	/* HTTP response status codes*/
// Information responses
# define HTTP_CONTINUE							100
# define HTTP_SWITCHING_PROTOCOLS 				101
# define HTTP_PROCESSING						102
# define HTTP_EARLY_HINTS						103
// Successful responses
# define HTTP_OK								200
# define HTTP_CREATED							201
# define HTTP_ACCEPTED							202
# define HTTP_NON_AUTHORITATIVE_INFORMATION		203
# define HTTP_NO_CONTENT						204
# define HTTP_RESET_CONTENT						205
# define HTTP_PARTIAL_CONTENT					206
// Redirection messages
# define HTTP_MULTIPLE_CHOICES					300
# define HTTP_MOVED_PERMANENTLY					301
# define HTTP_FOUND								302
# define HTTP_SEE_OTHER							303
# define HTTP_NOT_MODIFIED						304
# define HTTP_USE_PROXY							305
# define HTTP_TEMPORARY_REDIRECT				307
# define HTTP_PERMANENT_REDIRECT				308
// Client error responses
# define HTTP_BAD_REQUEST						400
# define HTTP_UNAUTHORIZED						401
# define HTTP_PAYMENT_REQUIRED					402
# define HTTP_FORBIDDEN							403
# define HTTP_NOT_FOUND							404
# define HTTP_METHOD_NOT_ALLOWED				405
# define HTTP_NOT_ACCEPTABLE					406
# define HTTP_PROXY_AUTHENTICATION_REQUIRED		407
# define HTTP_REQUEST_TIMEOUT					408
# define HTTP_CONFLICT							409
# define HTTP_GONE								410
# define HTTP_LENGTH_REQUIRED					411
# define HTTP_PRECONDITION_FAILED				412
# define HTTP_PAYLOAD_TOO_LARGE					413
# define HTTP_URI_TOO_LONG						414
# define HTTP_UNSUPPORTED_MEDIA_TYPE			415
# define HTTP_RANGE_NOT_SATISFIABLE				416
# define HTTP_EXPECTATION_FAILED				417
# define HTTP_UPGRADE_REQUIRED					426
# define HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE	431
// Server error responses
# define HTTP_INTERNAL_SERVER_ERROR				500
# define HTTP_NOT_IMPLEMENTED					501
# define HTTP_BAD_GATEWAY						502
# define HTTP_SERVICE_UNAVAILABLE				503
# define HTTP_GATEWAY_TIMEOUT					504
# define HTTP_VERSION_NOT_SUPPORTED				505

	/* limits for requets messages */
# define MAX_URI_LENGTH				2048
# define MAX_REQUEST_LINE_LENGTH	4096

# define MAX_HEADER_FIELDS			100
# define MAX_HEADER_NAME_LENGTH		100
# define MAX_HEADER_VALUE_LENGTH	1000
# define MAX_HEADERS_LENGTH			MAX_HEADER_FIELDS * (MAX_HEADER_NAME_LENGTH + MAX_HEADER_VALUE_LENGTH + 2)
# define DEFAULT_MAX_BODY_SIZE		1048576
# define MAXIMUM_MAX_BODY_SIZE		104857600

# define DEFAULT_PORT		80

# define CRLF				"\r\n"
# define CRLF_CRLF			"\r\n\r\n"
# define PROTOCOL			"http"

	/* request flags showing the status */
# define NO					0b00000000
# define YES				0b11111111
# define REQUEST_LINE		0b00000001
# define HEADERS			0b00000010
# define PROCESSED_HEADERS	0b00000100
# define BODY				0b00001000

enum e_methods {
	GET,
	POST,
	DELETE,
	PUT,
	NUMBER_OF_METHODS
};

#endif