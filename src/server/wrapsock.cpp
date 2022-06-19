#include "Server.hpp"

namespace ft
{
	int	Accept(int fd, struct sockaddr *sa, socklen_t *salenptr) {
		int	n;

		while ( (n = accept(fd, sa, salenptr)) < 0) {
	#ifdef	EPROTO
			if (errno != EPROTO && errno != ECONNABORTED)	// EPROTO protocol error
	#else
			if (errno != ECONNABORTED)						// ECONNABORTED software caused connection abort
	#endif
				ft::systemErrorExit("accept error");
		}
		return n;
	}

	void	Close(int fd) {
		if (close(fd) == -1)
			systemErrorExit("close error");
	}
}
