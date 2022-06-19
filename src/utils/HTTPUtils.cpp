#include "../../includes/utils.hpp"
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

namespace ft {
	std::string readFile(std::string file) {
		char buffer[BUFFER_SIZE + 1] = {0};
		int fd;
		int i;
		int res;
		std::string result = "";

		fd = open(file.c_str(), O_RDONLY);
		if (fd < -1) {
			throw FileError("File does not exist\n");
		}
		while ((res = read(fd, buffer, BUFFER_SIZE)) > 0) {
			result += buffer;
			i = 0;
			while (i < BUFFER_SIZE)
				buffer[i++] = 0;
		}
		if (res < 0) {
			throw FileError("Can not read file\n");
		}
		close(fd);
		return (result);
	}

	std::string IntToStr(size_t n) {
		std::ostringstream convert;

		convert << n;
		return (convert.str());
	}

	std::string replace(std::string source, std::string to_replace, std::string new_value) {
		size_t start_pos = 0;
		while ((start_pos = source.find(to_replace, start_pos)) != std::string::npos) {
			source.replace(start_pos, to_replace.length(), new_value);
			start_pos += new_value.length();
		}
		return (source);
	}

	int pathType(std::string path) {
		struct stat buffer;

		if (!stat(path.c_str(), &buffer)) {
			if (S_ISREG(buffer.st_mode))
				return (1);
			else
				return (2);
		} else
			return (0);
	}

	std::string TimeToStr(time_t timestamp)
	{
		char buffer[33];
		struct tm *ts;
		size_t last;

		ts   = localtime(&timestamp);
		last = strftime(buffer, 32, "%a, %d %b %Y %T GMT", ts);
		buffer[last] = '\0';
		return (std::string(buffer));
	}

	std::string getDate(void)
	{
		struct timeval now;
		struct timezone tz;

		gettimeofday(&now, &tz);
		return (TimeToStr(now.tv_sec + tz.tz_minuteswest * 60));
	}

	std::string getExpDate(void)
	{
		struct timeval now;
		struct timezone tz;

		gettimeofday(&now, &tz);
		return (TimeToStr(now.tv_sec + tz.tz_minuteswest * 60 + 1000000));
	}

	std::vector<unsigned char> readBinaryFile(std::string file) {
		char buffer[BUFFER_SIZE + 1] = {0};
		int fd;
		int i;
		int res;
		std::vector<unsigned char> result;

		fd = open(file.c_str(), O_RDONLY);
		if (fd < -1)
		{
			throw FileError("File does not exist");
		}
		while ((res = read(fd, buffer, BUFFER_SIZE)) > 0)
		{
			for (size_t j = 0; j < (size_t)res; ++j)
				result.push_back(buffer[j]);
			i = 0;
			while (i < BUFFER_SIZE)
				buffer[i++] = 0;
		}
		if (res < 0)
			throw FileError("Can not read file");
		close(fd);
		return (result);
	}
}