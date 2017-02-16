#pragma once

#include <string>
#include <vector>

#include "Types.h"

class Reporter
{
	std::vector<std::string> m_lostDirs;
	std::vector<ReportFile_t> m_badFiles;
	std::vector<ReportFile_t> m_goodFiles;

	time_t m_startDate;

public:
	Reporter();
	~Reporter();

	void startNow(); //updates m_startDate with cur time

	void addLostDir( std::string dir);
	void addBadFile( ReportFile_t file);
	void addGoodFile( ReportFile_t file);

	void printReport();
};
