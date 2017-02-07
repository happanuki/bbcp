#pragma once

#include <string>
#include <set>
#include <mutex>

#include "Types.h"

class Reporter
{
	std::mutex m_mtx;

	std::set<std::string> m_badDirs;

	std::set<std::string> m_badOffsets;

public:
	Reporter() {}

	void addBadDir( std::string dir) {}

};
