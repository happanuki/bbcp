#include <Logger.h>
#include <Syscalls.h>

#include "Udev.h"
#include "Wizard.h"

#include <sys/mount.h>
#include <stdlib.h>


Wizard::Wizard(AppParameters_t& p) :
	m_parameters(p)
{
}


void Wizard::_getHddFrom()
{
	LOGSTDOUT(" WIZARD == > " << " Insert BAD HDD...");

	m_parameters.diskDev = m_udev.recvDevice();
	m_parameters.partDev = m_parameters.diskDev + "1"; //first partition

	LOGSTDOUT(" WIZARD == > " << " BAD HDD       : " << m_parameters.diskDev);
	LOGSTDOUT(" WIZARD == > " << " BAD PARTITION : " << m_parameters.partDev);

}


void Wizard::_getDirTo()
{
	LOGSTDOUT(" WIZARD == > " << " Insert GOOD HDD...");

	auto hdd = m_udev.recvDevice();
	auto part = hdd + "1";

	System::mkdir_p(m_localMountPoint);

	(void) umount(m_localMountPoint.c_str());
	System::mount(part, m_localMountPoint,"exfat", MS_NOEXEC|MS_NOSUID);


	m_parameters.restoreDir = m_localMountPoint;

	std::string cmd("/usr/sbin/mount.exfat ");
	cmd+=" " + part + " " + m_localMountPoint;
	system(cmd.c_str());

	LOGSTDOUT(" WIZARD == > " << " GOOD HDD      : " << hdd);
	LOGSTDOUT(" WIZARD == > " << " GOOD DIR      : " << m_localMountPoint);
}


void Wizard::run()
{
	LOGSTDOUT("\t >>> WIZARD START <<<< ");

	m_udev.init();

	_getHddFrom();
	_getDirTo();
	_getRestoreMode();
	_getRunMode();

	m_parameters.storageFile = m_localMountPoint+"/restorer.file";

	LOGSTDOUT("\t >>> WIZARD COMPLETE <<<< ");
}


void Wizard::_getRestoreMode()
{
	m_parameters.restore_mode = RestoreMode_E::RESTORE_UNKNOWN;
	while ( m_parameters.restore_mode == RestoreMode_E::RESTORE_UNKNOWN ) {
		LOGSTDOUT(" WIZARD == > " << " Set restore mode [ 0* - only full-recoverable-files, 1 - recover all files [LONGMODE], ENTER selects 0");

		char m = 0;
		std::cin >> m;

		if (m == '1') {
			m_parameters.restore_mode = RestoreMode_E::RESTORE_ALL;
		}
		else if (m == '0' || (!m) ) {
			m_parameters.restore_mode = RestoreMode_E::RESTORE_ONLY_GOOD;
		}
		else {
			LOGSTDOUT(" WIZARD == > " << " FU");
		}
	}
}


void Wizard::_getRunMode()
{
	m_parameters.app_mode = AppMode_E::APP_BAD_MODE;
	while ( m_parameters.app_mode == AppMode_E::APP_BAD_MODE ) {
		LOGSTDOUT(" WIZARD == > " << " 0* - CONTINUE   1 - start again");

		char m;
		std::cin >> m;

		if (m == '1') {
			m_parameters.app_mode = AppMode_E::APP_START;
		}
		else if (m == '0' || (!m) ) {
			m_parameters.app_mode = AppMode_E::APP_CONTINUE;
		}
		else {
			LOGSTDOUT(" WIZARD == > " << " FU");
		}

	}
}

