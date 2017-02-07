#pragma once

#include <exception>

#include "Types.h"

class ArgsParser
{
	int m_argc = 0;
	char** m_argv = nullptr;
	char** m_envp = nullptr;

	AppParameters m_parameters;

	void _parseArgs();
	void _runWizzard();

public:
	ArgsParser(int argc, char** argv, char** envp);

	AppParameters get() const { return m_parameters; }

	void printHelp();
};
