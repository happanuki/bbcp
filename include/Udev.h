#pragma once

#include "libudev.h"

#include <exception>
#include <vector>
#include <string>

#include <Types.h>


class Udev
{
	struct udev* m_udev = nullptr;
	struct udev_monitor* m_mon = nullptr;
	int m_monFd = -1;

	std::string m_sysClass = "block";
	std::string m_action = "add";
	std::string m_type = "disk";

	void _uninit();

	void _setupMonitor();

public:
	Udev();
	~Udev();

	void init() throw (std::exception&);

	std::string recvDevice() throw (std::exception&);

};
