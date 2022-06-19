#include <sys/stat.h>
#include "utils.hpp"

namespace ft {
	void	goForwardToken(std::vector<std::string>::iterator &it,
					std::vector<std::string>::iterator &end,
					size_t n, const char *neededToken)
	{
		while (it != end && n-- > 0)
			++it;
		if (it == end || (neededToken && it->compare(neededToken)))
			ft::errorExit("Invalid config file");
	}

	void	validateDirectoryPath(const std::string &dirPath) {
		struct stat statbuf;

		if (stat(dirPath.data(), &statbuf) < 0)
			ft::systemErrorExit("can't open dir: " + dirPath);
		if (!S_ISDIR(statbuf.st_mode))
			ft::errorExit(dirPath + " is not a dirrectory");
	}

	void	validateFilePath(const std::string &filePath) {
		struct stat statbuf;

		if (stat(filePath.data(), &statbuf) < 0)
			ft::systemErrorExit("can't open file: " + filePath);
		if (!S_ISREG(statbuf.st_mode))
			ft::errorExit(filePath + " is not a file");
	}
}