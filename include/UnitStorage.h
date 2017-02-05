#pragma once

#include "Unit.h"
#include <exception>

class UnitStorage
{
	std::string m_storeFile;
	uint64_t m_storeFileSize = 0;

	bool m_isInit = false;

	std::shared_ptr<void> m_mmap;

	std::shared_ptr<void> _initMmap() throw (std::exception&); //mmap m_storeFile
public:

	UnitStorage(std::string filename);

	void init();
	bool isInit() const { return m_isInit; }

	void store(const std::vector<Unit>&);
	std::vector<Unit> load();
};
