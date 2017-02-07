#include <unistd.h>
#include <getopt.h>

#include <Logger.h>
#include <Syscalls.h>

#include "Application.h"
#include "ExfatPartition.h"
#include "UnitStorage.h"
#include "Processor.h"


Application::Application(AppParameters in):
		m_parameters(in)
{
}


void Application::_validateParams() throw (std::exception&)
{
	if ( m_parameters.mode == AppMode_E::APP_BAD_MODE) {
		THROW_LOGIC_EXCEPTION("Bad application mode ");
	}

	if ( m_parameters.restoreDir.empty()) {
		THROW_LOGIC_EXCEPTION("No restore dir given");
	}

	if ( m_parameters.partDev.empty()) {
		THROW_LOGIC_EXCEPTION("No partition device given");
	}
}


void Application::_validateDevFiles() throw (std::exception&)
{
	if (! System::isFileExist( m_parameters.partDev)) {
		THROW_SYS_EXCEPTION("file " << m_parameters.partDev << " not found");
	}

	if (! System::isFileExist( m_parameters.diskDev)) {
		WARNSTDOUTT("no disk file, skipping hw setup for hdd");
	}
	else {
		m_initHdd = true;
	}
}


void Application::_setHddParameters() throw (std::exception&)
{
	m_drive.reset( new Drive(System::basename_(m_parameters.diskDev)));
	m_drive->init();
}


void Application::init()
{
	_validateParams();
	_validateDevFiles();

	if (m_initHdd) {
		_setHddParameters();
	}

}

void Application::_genericRun()
{
	UnitStorage storage( m_parameters.storageFile);
	storage.init();

	auto units = storage.load();

	Processor p( units, m_parameters.partDev, m_parameters.restoreDir);

	p.process();
}


void Application::run()
{
	if ( m_parameters.mode == AppMode_E::APP_START) {
		ExfatPartition ef( m_parameters.partDev);
		ef.init();
		UnitStorage storage( m_parameters.storageFile);

		storage.store( ef.getUnitList());
	}

	_genericRun();

}
