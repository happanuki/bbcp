#pragma once

#include <string>
#include <exception>

#include "Unit.h"

class Processor
{
	std::vector<Unit> m_units;
	std::string m_devFile;
	std::string m_outDir;

	int m_devFd = 0;

	void _processUnit(Unit&);
	void _recoverUnit(std::string path, Unit&);
//	void _mkdir(std::string pathName) throw (std::exception&);

public:
	Processor(const std::vector<Unit>&, std::string devFile, std::string outDir);
	~Processor();

	void process();
};
