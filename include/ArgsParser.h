#pragma once

#include <exception>

#include "Types.h"

class ArgsParser
{
	int m_argc = 0;
	char** m_argv = nullptr;
	char** m_envp = nullptr;

	AppParameters_t m_parameters;

	void _parseArgs();
	void _runWizzard();

public:
	ArgsParser(int argc, char** argv, char** envp);

	AppParameters_t get() const { return m_parameters; }

	void printHelp();
};
