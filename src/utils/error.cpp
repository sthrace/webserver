#include <stdio.h>
#include "utils.hpp"

namespace ft
{	
	/*
	**	Fatal error related to a system call.
	**	Print a message and terminate.
	*/

	void systemErrorExit(const std::string& mesage)
	{
		timestamp("System error exit: " + mesage);
		std::cerr << RED_COLOR;
		perror(mesage.c_str());
		std::cerr << RESET_COLOR << std::endl;
		exit(EXIT_FAILURE);
	}

	/*
	**	Fatal error unrelated to a system call.
	**	Print a message and terminate.
	*/

	void errorExit(const std::string& mesage)
	{
		timestamp("Error exit: " + mesage);
		std::cerr << RED_COLOR  << mesage << RESET_COLOR << std::endl;
		exit(EXIT_FAILURE);
	}
} // namespace ft
