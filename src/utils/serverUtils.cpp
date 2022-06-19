#include <fstream>
#include <iomanip>
#include "utils.hpp"

namespace ft
{	
	/*
	* This function writes a timestamp string to the open fileLog 
	*/ 
	void timestamp(const std::string& msg) {
		std::fstream	fout("./logs/webserver.log", std::ios_base::out | std::ios_base::app);

		if ((fout.rdstate() & std::ifstream::failbit) != 0 )
			std::cerr << "Error opening 'webserver.log'" << std::endl;
		else {
			time_t timer = time(&timer);
			// fout << std::put_time(localtime(&timer), "%d/%m/%Y %H:%M:%S> ")			// Since C++11
			// 		<< msg << '\n';
			struct tm *tmp = localtime(&timer);						// For c++98 standart
			fout << tmp->tm_mday << '/' << tmp->tm_mon + 1 << '/' << tmp->tm_year + 1900
				<< ' ' << tmp->tm_hour << ':' << tmp->tm_min << ':' << tmp->tm_sec << ' '
				<< msg << '\n';
		}
		if ((fout.rdstate() & std::ifstream::badbit) != 0 ) {
			std::cerr << "Writing 'webserver.log' error" << std::endl;
		}
		fout.close();
	}

	std::string	Getcwd() {
		char		*cwd;
		std::string	currentDir;

		cwd = getcwd(NULL, 0);
		if (!cwd)
			ft::systemErrorExit("getcwd");
		currentDir = cwd;
		free(cwd);
		return currentDir;
	}
}
