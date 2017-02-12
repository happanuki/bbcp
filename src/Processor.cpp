#include <libgen.h> //dirname(), basename()
#include <unistd.h>

#include <Syscalls.h>
#include <Logger.h>

#include "Processor.h"
#include "Exception.h"


Processor::Processor(const std::vector<Unit>& units, std::string devFile, std::string outDir,RestoreMode_E m) :
	m_units(units),
	m_devFile(devFile),
	m_outDir(outDir),
	m_restoreMode(m)
{
	m_devFd = System::open(m_devFile.c_str(),O_RDWR,0);
}


Processor::~Processor()
{
	close(m_devFd);
}

void Processor::_processUnit(Unit& in)
{
	if ( in.isProcessed()) {
		LOGSTDOUTT("skip processing of " << in.getFile()->getName());
		return;
	}

	auto dir = System::dirname_(in.getFile()->getName()); //std::string( dirname(const_cast<char*>( in.getFile()->getName().c_str())));

	auto fname = System::basename_(in.getFile()->getName()); //std::string( basename(const_cast<char*>( in.getFile()->getName().c_str())));

	auto restoreDirName = m_outDir + "/" + dir;
	System::mkdir_p(restoreDirName); //_mkdir(restoreDirName);

	auto outFileName = restoreDirName + "/" + fname ;
	DEBUGSTDOUT("Processing " << in.getFile()->getName() << " restoring to : " << outFileName);

	_recoverUnit(outFileName,in);

	in.setProcessed(true);
	in.syncMe();
}


void Processor::_recoverUnit(std::string path, Unit& u)
{
	auto recoverFd = System::openAutoClose(path.c_str(),O_RDWR|O_CREAT|O_TRUNC, 0644);

	for (auto& it: u.getFile()->getOffsets() ) {
		if (! it.offset) {
			LOGSTDOUTT("bad offset in " << u.getFile()->getName() << " [ " << it.offset << " ]");
			continue;
		}

		std::string sbuf(it.size, 0);

		char* buf = const_cast<char*>( sbuf.data());

		try {
			System::pread(m_devFd, buf, it.size, it.offset);
		}
		catch (std::exception& e) {
			//TODO: norm impl
			LOGSTDOUTT("Can't read devFile, offs: [" << it.offset << "]  file: " << u.getFile()->getName());

			if ( m_restoreMode == RestoreMode_E::RESTORE_ONLY_GOOD) {
				break;
			}
			else if (m_restoreMode == RestoreMode_E::RESTORE_ALL) {
			}
		}

		System::write( *recoverFd, buf, it.size);
	}
}


void Processor::process()
{
	for (auto& it: m_units) {
		_processUnit(it);
	}
}
