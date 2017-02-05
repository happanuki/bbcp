#pragma once

#include <Exception.h>

#include "Drive.h"

enum class AppMode_E
{
	APP_START,
	APP_CONTINUE,
	APP_BAD_MODE
};

class Application
{
	int m_argc;
	char** m_argv;

	std::string m_appName;

	std::string m_diskDev;
	std::string m_partDev;
	std::string m_storageFile = "storage.file";
	std::string m_restoreDir;

	AppMode_E m_mode = AppMode_E::APP_BAD_MODE;

	std::shared_ptr<Drive> m_drive;

	void _parseArgs();
	void _printHelp();

	void _validateParams() throw (std::exception&);
	void _validateDevFiles() throw (std::exception&);

	void _setHddParameters() throw (std::exception&);

	bool m_initHdd = false;

public:
	Application(int argc, char** argv);

	void init(); // throw (std::exception&);

	void run();
};
