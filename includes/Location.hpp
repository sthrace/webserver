#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <vector>
# include "HTTPConstants.hpp"
# include "utils.hpp"

namespace ft {
	class Location {
		public:
			Location(const std::string& currentDir = "");
			Location(const Location &other);
			~Location();

			Location &operator=(const Location &other);

			void	parseLocation(std::vector<std::string>::iterator &it,
						std::vector<std::string>::iterator &end,
						const std::string &curDir);
			void	setRoot(std::vector<std::string>::iterator &it,
						std::vector<std::string>::iterator &end);
			void	setMethods(std::vector<std::string>::iterator &it,
						std::vector<std::string>::iterator &end);
			void	setAutoindex(std::vector<std::string>::iterator &it,
						std::vector<std::string>::iterator &end);
			void	setUploadPermission(std::vector<std::string>::iterator &it,
					std::vector<std::string>::iterator &end);
			void	setIndex(std::vector<std::string>::iterator &it,
						std::vector<std::string>::iterator &end);
			void	setClientMaxBodySize(std::vector<std::string>::iterator &it,
						std::vector<std::string>::iterator &end);
			void	setFastcgiPass(std::vector<std::string>::iterator &it,
						std::vector<std::string>::iterator &end);
			void	setReturn(std::vector<std::string>::iterator &it,
						std::vector<std::string>::iterator &end);

			const std::string				&getRoot() const;
			const std::string				&getReturn() const;
			const std::vector<bool>			&getMethods() const;
			bool							getAutoindex() const;
			bool							getUploadPermission() const;
			const std::vector<std::string> 	&getIndex() const;
			const std::string				&getFastcgiPass() const;
			unsigned long					getClientMaxBodySize() const;

		private:
			std::string					_root;
			std::string					_return;
			std::vector<bool>			_methods;
			bool						_autoindex;
			bool						_uploadPermission;
			std::vector<std::string> 	_index;
			std::string					_fastcgiPass;
			unsigned long     			_clientMaxBodySize;
	};
}


#endif