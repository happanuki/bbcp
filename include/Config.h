#pragma once

#include <string>
#include <map>

using Config = std::map< std::string, std::string >;

class ConfigFactory
{
public:
	static Config getConfig();

};
