#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <Exception.h>
#include <Logger.h>

#include "Udev.h"


Udev::Udev()
{
}

Udev::~Udev()
{
	_uninit();
}


void Udev::init() throw (std::exception&)
{
	m_udev = udev_new();
	if (!m_udev) {
		THROW_LOGIC_EXCEPTION("libudev API : udev_new failed");
	}

	m_mon = udev_monitor_new_from_netlink(m_udev, "udev");
	if (!m_mon) {
		THROW_LOGIC_EXCEPTION("libudev API : udev_monitor_new failed");
	}

	udev_monitor_filter_add_match_subsystem_devtype(m_mon, m_sysClass.c_str(), NULL);

	m_monFd = udev_monitor_get_fd(m_mon);

	udev_monitor_enable_receiving(m_mon);
}

void Udev::_uninit()
{
	if (m_udev != nullptr) {
		udev_unref(m_udev);
	}
}

std::string Udev::recvDevice() throw (std::exception&)
{
	while( true ) {
		fd_set fds;
		struct timeval tv;
		int ret;

		FD_ZERO(&fds);
		FD_SET(m_monFd, &fds);
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		ret = select(m_monFd+1, &fds, NULL, NULL, &tv);

		/* Check if our file descriptor has received data. */
		if (ret > 0 && FD_ISSET(m_monFd, &fds)) {

			struct udev_device* dev = udev_monitor_receive_device(m_mon);
			if ( dev == nullptr ) {
				THROW_SYS_EXCEPTION("libudev API : udev_monitor_receive_device failed");
			}

			std::string l_act ( udev_device_get_action(dev));
			std::string l_path ( udev_device_get_devnode(dev));
			std::string l_type ( udev_device_get_devtype(dev));

//			DEBUGSTDOUTT("udev_device_get_action :" << udev_device_get_action(dev));
//			DEBUGSTDOUTT("udev_device_get_sysname :" << udev_device_get_sysname(dev));
//			DEBUGSTDOUTT("udev_device_get_syspath :" << udev_device_get_syspath(dev));
//			DEBUGSTDOUTT("udev_device_get_devnode :" << udev_device_get_devnode(dev));
//			DEBUGSTDOUTT("udev_device_get_devtype :" << udev_device_get_devtype(dev));


			if ( (l_act == m_action) && (l_type == m_type) ) {
				udev_device_unref(dev);
				return std::string(l_path);
			}
			else {
				continue;
			}
		}
		usleep(250*1000);
		printf(".");
		fflush(stdout);
	}
}

