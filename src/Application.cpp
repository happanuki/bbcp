#include <unistd.h>
#include <getopt.h>

#include <Logger.h>
#include <Syscalls.h>

#include "Application.h"
#include "ExfatPartition.h"
#include "UnitStorage.h"
#include "Processor.h"



Application::Application(int argc, char** argv) :
	m_argc(argc),
	m_argv(argv)
{
	m_appName = System::basename_( m_argv[0]);
}


void Application::_parseArgs()
{
	const char* short_options = "hd:p:s:r:m:";
	const option long_options[] = {
			{"help",no_argument,NULL,'h'},

			{"diskDev",optional_argument, NULL ,'d'},
			{"partDev",required_argument, NULL, 'p'},

			{"storageFile",optional_argument, NULL, 's'},
			{"restoreDir",required_argument, NULL, 'r'},

			{"mode", required_argument, NULL,'m'},

			{NULL,0,NULL,0}
	};
	int res,option_index;

	while ( (res=getopt_long(m_argc,m_argv,short_options,long_options,&option_index)) != -1) {

		bool exit = false;
		switch(res) {

		case 'd' : {
			m_diskDev = std::string(optarg);
		} break;

		case 'p' : {
			m_partDev = std::string(optarg);
		} break;

		case 's' : {
			m_storageFile = std::string(optarg);
		} break;

		case 'r' : {
			m_restoreDir = std::string(optarg);
		} break;

		case 'm' : {
			auto val = atoi(optarg);

			switch(val) {
			case 0:
				m_mode = AppMode_E::APP_START;
				break;
			case 1:
				m_mode = AppMode_E::APP_CONTINUE;
				break;
			default:
				m_mode = AppMode_E::APP_BAD_MODE;
			}
		} break;

		case 'h':
			exit = true;
		default :
			_printHelp();
			if (exit) {
				::exit(0);
			}
		}
	}
}


void Application::_printHelp()
{
	Logger::getInstance().setLogSTDOUT();
	Logger::getInstance().log() << " Usage: " <<  m_appName << " : " <<
			"\n\t" << "[ -d, --diskDev     ]\t " << " Disk device file                  Example: -d /dev/sda" <<
			"\n\t" << "[ -p, --partDev     ]\t " << " Partition device file             Example: -p /dev/sda1" <<
			"\n\t" << "[ -s, --storageFile ]\t " << " Processing status storage file    Example: -s status.sda1" <<
			"\n\t" << "[ -m, --mode        ]\t " << " Processing mode                   Example: -m 0" <<
			"\n\t" << "                        " << "        Available modes: " <<
			"\n\t" << "                        " << "             0 - start new full processing of the partition" <<
			"\n\t" << "                        " << "             1 - continue processing partition using status file" <<
			"\n\t" << "[ -r, --restoreDir  ]\t " << " Restore directory                 Example: -r /tmp/disk" <<
			"\n" << std::endl;
}


void Application::_validateParams() throw (std::exception&)
{
	if ( m_mode == AppMode_E::APP_BAD_MODE) {
		_printHelp();
		THROW_LOGIC_EXCEPTION("Bad application mode ");
	}

	if ( m_restoreDir.empty()) {
		THROW_LOGIC_EXCEPTION("Empty restore dir");
	}

	if ( m_partDev.empty()) {
		THROW_LOGIC_EXCEPTION("Empty partition device");
	}

}


void Application::_validateDevFiles() throw (std::exception&)
{
	if (! System::isFileExist(m_partDev)) {
		THROW_SYS_EXCEPTION("file " << m_partDev << " not found");
	}

	if (! System::isFileExist(m_diskDev)) {
		WARNSTDOUTT("no disk file, skipping hw setup for hdd");
	}
	else {
		m_initHdd = true;
	}
}


void Application::_setHddParameters() throw (std::exception&)
{
	m_drive.reset( new Drive(System::basename_(m_diskDev)));
	m_drive->init();
}


void Application::init()
{
	_parseArgs();
	_validateParams();
	_validateDevFiles();

	if (m_initHdd) {
		_setHddParameters();
	}

}


void Application::run()
{
	if ( m_mode == AppMode_E::APP_START) {
		ExfatPartition ef( m_partDev);
		ef.init();
		UnitStorage storage( m_storageFile);

		storage.store(ef.getUnitList());
	}

	UnitStorage storage( m_storageFile);
	storage.init();

	auto units = storage.load();

	Processor p(units,m_partDev,m_restoreDir);

	p.process();

}
