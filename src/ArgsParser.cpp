#include <unistd.h>
#include <getopt.h>

#include <Logger.h>

#include "ArgsParser.h"


ArgsParser::ArgsParser(int argc, char** argv, char** envp) :
	m_argc(argc),
	m_argv(argv),
	m_envp(envp)
{
	_parseArgs();
}


void ArgsParser::printHelp()
{
	Logger::getInstance().setLogSTDOUT();
	Logger::getInstance().log() << " Usage: " <<  m_parameters.appName << " : " <<
			"\n\t" << "[ -w, --wizard      ]\t " << " Wizard Mode                       Example: -w" <<
			"\n\t" <<
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


void ArgsParser::_parseArgs()
{
	m_parameters.appName = System::basename_( m_argv[0]);

	const char* short_options = "hwd:p:s:r:m:";
	const option long_options[] = {
			{"help",no_argument,NULL,'h'},

			{"wizard", no_argument, NULL,'w'},

			{"diskDev",required_argument, NULL ,'d'},
			{"partDev",required_argument, NULL, 'p'},

			{"storageFile",required_argument, NULL, 's'},
			{"restoreDir",required_argument, NULL, 'r'},

			{"mode", required_argument, NULL,'m'},

			{NULL,0,NULL,0}
	};
	int res,option_index;

	while ( (res=getopt_long(m_argc,m_argv,short_options,long_options,&option_index)) != -1) {

		bool exit = false;
		bool wizzard = false;

		switch(res) {

		case 'w' : {
			wizzard = true;
		} break;

		case 'd' : {
			m_parameters.diskDev = std::string(optarg);
		} break;

		case 'p' : {
			m_parameters.partDev = std::string(optarg);
		} break;

		case 's' : {
			m_parameters.storageFile = std::string(optarg);
		} break;

		case 'r' : {
			m_parameters.restoreDir = std::string(optarg);
		} break;

		case 'm' : {
			auto val = atoi(optarg);

			switch(val) {
			case 0:
				m_parameters.mode = AppMode_E::APP_START;
				break;
			case 1:
				m_parameters.mode = AppMode_E::APP_CONTINUE;
				break;
			default:
				m_parameters.mode = AppMode_E::APP_BAD_MODE;
			}
		} break;

		case 'h':
			exit = true;
		default :
			printHelp();
			if (exit) {
				::exit(0);
			}
		}

		if ( wizzard) {
			_runWizzard();
			break; //while break
		}

	} // while
}


void ArgsParser::_runWizzard()
{

}
