#pragma once

#include <exception>
#include <string>

#include "Config.h"

class Drive
{
	std::string m_devName; // = "sde" in constructor
	bool m_isRdy = false;

	int m_minIO_sz = 512; //minimum I/O size
	int m_sector_sz = 512; //sector size

	void _applyConfig(const Config&);
	void _setSelfParameters();

public:

	/*
	 * returns -1 and errno on "error"
	 */
	static int adoptiveReadWrite(int fd_read, int fd_write, off_t offset, int min_bytes=512, int good_bytes=4096);

	Drive() = delete;
	Drive(const std::string& devName = "sde");
	~Drive();

	inline bool isRdy() const { return m_isRdy; }
	void init() throw (std::exception&);
	void uninit() noexcept;

	off_t getSize();
};
