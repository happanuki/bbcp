#pragma once

#include <Exception.h>

#include "Types.h"
#include "Drive.h"

class Application
{
	AppParameters m_parameters;

	std::shared_ptr<Drive> m_drive;

	void _validateParams() throw (std::exception&);
	void _validateDevFiles() throw (std::exception&);

	bool m_initHdd = false;
	void _setHddParameters() throw (std::exception&);

	void _genericRun();

public:
	Application(AppParameters);

	void init(); // throw (std::exception&);

	void run();
};
