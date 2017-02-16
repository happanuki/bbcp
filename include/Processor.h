#pragma once

#include <string>
#include <exception>

#include "Unit.h"
#include "Types.h"
#include "Reporter.h"

class Processor
{
	std::vector<Unit> m_units;
	std::string m_devFile;
	std::string m_outDir;

	RestoreMode_E m_restoreMode;

	int m_devFd = 0;

	void _processUnit(Unit&);
	void _recoverUnit(std::string path, Unit&);

public:
	Processor(const std::vector<Unit>&, std::string devFile, std::string outDir,RestoreMode_E m);
	~Processor();

	void process();
};
