#include "utils.hpp"
#include <sstream>

namespace ft
{	
	bool	space(char c) {
		return std::isspace(c);
	}
	
	bool	notSpace(char c) {
		return !std::isspace(c);
	}
	
	/*
	**	splits std::string into std::vector<std::string> by whitespaces
	*/
	std::vector<std::string>	split(const std::string& str) {
		std::string::const_iterator	itBegin;
		std::string::const_iterator	itEnd;
		std::vector<std::string>	splitedStrings;

		itBegin = str.begin();
		while((itBegin = std::find_if(itBegin, str.end(), notSpace)) != str.end()) {
			itEnd = std::find_if(itBegin, str.end(), space);
			splitedStrings.push_back(std::string(itBegin, itEnd));
			itBegin = itEnd;
		}
		return splitedStrings;
	}

	/*
	**	splits std::string into std::vector<std::string> by delimeter
	*/
	std::vector<std::string> split(const std::string& str,
                                    const std::string& delim)
	{
    	std::vector<std::string>	splitedStrings;
		std::string::size_type		pos = 0;
		std::string::size_type		prev = 0;
		std::string::size_type		len = str.length();

		do {
			pos = str.find(delim, prev);
			if (pos != prev)
				splitedStrings.push_back(str.substr(prev, pos - prev));
			prev = pos != std::string::npos ? pos + delim.length() : std::string::npos;
		} while (prev < len);
    	return splitedStrings;
	}

	/*
	**	Converts a string to upper sase
	*/
	void	toUpperString(std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(), toupper);
	}

	/*
	**	Converts a string to lower sase
	*/
	void	toLowerString(std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(), tolower);
	}

	/*
	**	Returns remainder of the string after delimeter
	*/
	std::string	getExtension(const std::string& str,
                                const std::string& delim, size_t pos)
	{
		std::string::size_type		found;

		found = str.find(delim, pos);
		if (found == std::string::npos)
        	return "";
   		else
        	return str.substr(pos, found + delim.length() - pos);
	}

	/*
	**	Returns the begining of the string before delimeter
	*/
	std::string	getWithoutExtension(const std::string& str,
                                    	const std::string& delim, size_t pos)
	{
		std::string::size_type		found;

		found = str.find(delim, pos);
        return str.substr(pos, found - pos);
	}

	/*
	**	Returns true if string contains only digits
	*/
	bool isNumber(const std::string& str) {
		std::string::size_type len = str.size();

		for (std::string::size_type i = 0; i < len; ++i) {
        	if (!std::isdigit(str[i]))
				return false;
    	}
   		return true;
	}

	/*
	** Parses token in the string before delim. Returns true if token parsed.
	*/
	bool	parseToken(const std::string& src, const std::string &delim,
				std::string::size_type& beginPos, std::string &token,
				bool skipFirstWhiteSpaces, bool isExactDelim,
				std::string::size_type maxLen)
	{
		token.clear();
		if (skipFirstWhiteSpaces)
			beginPos = src.find_first_not_of(WHITESPACES, beginPos);
		if (beginPos >= src.length())
			return false;
		std::string::size_type endPos = src.find(delim, beginPos);
		if (endPos == std::string::npos && isExactDelim)
			return false;
		if (endPos == std::string::npos)
			endPos = src.length();
		token = src.substr(beginPos, endPos - beginPos);
		beginPos = endPos != src.length() ? endPos + delim.length() : endPos;
		if (token.length() > maxLen || token.length() == 0)
			return false;
		return true;
	}
	
	/*
	**	Converts a numeric value to std::string
	*/
	std::string to_string (int val) {
		std::stringstream ss;
		
		ss << val;
		return ss.str();
	}

	std::string to_string (long val) {
		std::stringstream ss;
		
		ss << val;
		return ss.str();
	}

	std::string to_string (long long val) {
		std::stringstream ss;
		
		ss << val;
		return ss.str();
	}

	std::string to_string (unsigned val) {
		std::stringstream ss;
		
		ss << val;
		return ss.str();
	}

	std::string to_string (unsigned long val) {
		std::stringstream ss;
		
		ss << val;
		return ss.str();
	}

	std::string to_string (unsigned long long val) { 
		std::stringstream ss;
		
		ss << val;
		return ss.str();
	}
}