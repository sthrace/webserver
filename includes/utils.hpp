#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <cstring>
# include <cstdlib>
# include <unistd.h>
# include <errno.h>
# include <sys/types.h>	// basic system data types
# include <sys/socket.h>	// basic socket definitions

# define RED_COLOR		"\033[1;31m"
# define GREEN_COLOR	"\033[0;32m"
# define RESET_COLOR 	"\033[0;0m"

# define WHITESPACES	" \t\n\v\f\r"
# define BUFFER_SIZE 256

namespace ft
{
    void						systemErrorExit(const std::string& mesage);
    void						errorExit(const std::string& mesage);
    int							Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
	void						Close(int fd);
	std::vector<std::string>	split(const std::string& str);
	std::vector<std::string> 	split(const std::string& str, const std::string& delim);
	void						toUpperString(std::string& str);
	void						toLowerString(std::string& str);
	std::string					getExtension(const std::string& str, const std::string& delim,
											size_t pos = 0);
	std::string					getWithoutExtension(const std::string& str,
													const std::string& delim, size_t pos = 0);
	bool						isNumber(const std::string& str);
	bool						parseToken(const std::string& src, const std::string &delim,
										std::string::size_type& beginPos, std::string &token,
										bool skipFirstWhiteSpaces = true, bool isExactDelim = false,
										std::string::size_type maxLen = std::string::npos);
	void						timestamp(const std::string& msg);
	std::string 				Getcwd();
	void						goForwardToken(std::vector<std::string>::iterator &it,
									std::vector<std::string>::iterator &end,
									size_t n, const char *neededToken = NULL);
	void						validateDirectoryPath(const std::string &dirPath);
	void						validateFilePath(const std::string &filePath);

	std::string 				to_string (int val);
	std::string 				to_string (long val);
	std::string					to_string (long long val);
	std::string 				to_string (unsigned val);
	std::string					to_string (unsigned long val);
	std::string					to_string (unsigned long long val);

	std::string                 readFile(std::string file);
	std::vector<unsigned char>	readBinaryFile(std::string file);
	std::string                 IntToStr(size_t n);
	std::string 				replace(std::string source, std::string to_replace, std::string new_value);
	int 						pathType(std::string path);
	std::string 				getDate(void);
	std::string 				getExpDate(void);
	std::string 				TimeToStr(time_t timestamp);

	class FileError : public std::exception
	{
	private:
		std::string _msg;
	public:
		FileError(std::string msg) : _msg(msg) {};
		~FileError() throw()
		{};
		const char *what () const throw () { return (_msg.c_str()); };
	};
}
#endif