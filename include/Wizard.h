#pragma once

#include "Types.h"
#include "Udev.h"

class Wizard
{
	AppParameters_t& m_parameters;
	Udev m_udev;

	std::string m_localMountPoint = "mnt";

	void _getHddFrom();
	void _getDirTo();
	void _getRestoreMode();
	void _getRunMode();

public:
	Wizard(AppParameters_t&);
	~Wizard() = default;

	void run();
};
