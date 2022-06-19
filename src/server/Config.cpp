#include "Config.hpp"

ft::Config::Config(const std::string& configFile) {
	parse(configFile);
}

ft::Config::~Config() {}

void ft::Config::splitTokens(const std::string& configFile,
		std::vector<std::string>& tokens) {

    std::fstream	fin(configFile.c_str(), std::ios::in | std::ios::binary);
    std::string		line;
	std::string 	token;

	if ((fin.rdstate() & std::ios::failbit) != 0)
        ft::errorExit("Error opening " + configFile);
	while (std::getline(fin, line)) {
		token.clear();
		size_t len = line.size();
		for (size_t i = 0; i < len; ++i) {
			if (line[i] != ';' && line[i] != '#' && !std::isspace(line[i])) {
				token += line[i];
				continue;
			}
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
			if (line[i] == ';')
				tokens.push_back(std::string(1, line[i]));
			else if (line[i] == '#')
				break;
		}
		if (!token.empty())
			tokens.push_back(std::string(token));
	}
    fin.close();
}

void	ft::Config::hasMinimumParameters(const ft::VirtualHost &virtualHost) {
	std::string	msg;

	if (virtualHost.getHost() == INADDR_NONE)
		msg.append("Specify host in config file\n");
	if (virtualHost.getServerName().empty())
		msg.append("Specify server name in config file\n");
	if (!msg.empty())
		ft::errorExit(msg);
}

void	ft::Config::validateParentheses(std::vector<std::string> &tokens,
			const std::string &openParenthesis, const std::string &closingParenthesis)
{
	int balance = 0;

	std::vector<std::string>::iterator it = tokens.begin();
	std::vector<std::string>::iterator end = tokens.end();
	while (it != end) {
		if (*it == openParenthesis)
			++balance;
		else if (*it == closingParenthesis)
			--balance;
		if (balance < 0)
			ft::errorExit("Invalid parentheses");
		++it;
	}
	if (balance != 0)
		ft::errorExit("Invalid parentheses");
}

void	ft::Config::parse(const std::string& configFile) {
	std::vector<std::string>	tokens;

    splitTokens(configFile, tokens);
	validateParentheses(tokens, "{", "}");
	std::string currentDir = Getcwd();
	std::vector<std::string>::iterator it = tokens.begin();
	std::vector<std::string>::iterator end = tokens.end();
    for ( ; it < end; ++it) // parsing servers
    {
        if (*it != "server" && *(++it) != "{")
            ft::errorExit("Invalid config file");
        ft::VirtualHost virtualHost(currentDir);
        while (++it != end && *it != "}") // parsing inside server
        {
            if (*it == "host") {
				virtualHost.setHost(it, end);
            } else if (*it == "port") {
				virtualHost.setPort(it, end);
            } else if (*it == "server_name") {
				virtualHost.setServerName(it, end);
            } else if (*it == "error_page") {
				virtualHost.setErrorPage(it, end);
            } else if (*it == "client_max_body_size") {
				virtualHost.setClientMaxBodySize(it, end);
            } else if (*it == "location") {
				virtualHost.setLocation(it, end);
            }
        }
        if (it == end)
			ft::errorExit("Invalid config file. Missing }");
		hasMinimumParameters(virtualHost);
		_virtualHosts[virtualHost.getServerName()] = virtualHost;
    }
}

const std::map<std::string, ft::VirtualHost> &ft::Config::getVirtualHosts() const {
	return _virtualHosts;
}
