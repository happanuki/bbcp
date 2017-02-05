#pragma once

#include "sys/types.h"

#include <exception>
#include <mutex>
#include <memory>
#include <vector>

#include "exfat.h"
#include "ExfatFile.h"
#include "Unit.h"


class ExfatPartition
{
	const std::string m_mountOpts = "ro";
	std::string m_devFileName;
	bool m_init = false;
	std::mutex m_mtx;

	std::shared_ptr<exfat> m_exfat;

	off_t m_sectorOffset = 0;
	off_t _getSectorOffset() throw (std::exception&);

	off_t m_cluster_size = 0;

	void _dirsRecursiveWalk(exfat_node* startNode, std::string curPath, std::vector<ExfatFile>& out);

	void uninit() noexcept;

public:

	ExfatPartition( std::string devFileName = "" ); // C++11 style, r-val construction
	~ExfatPartition() {}

	void init() throw(std::exception&); // thread-safe
	bool isInit() const { return m_init; }

	std::shared_ptr<exfat> getHandler() { return m_exfat; }

	std::vector<ExfatFile> getFileList();
	std::vector<Unit> getUnitList();

};
