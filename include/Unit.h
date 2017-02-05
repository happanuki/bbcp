#pragma once

#include "interfaces/File_I.h"

class Unit
{
	std::shared_ptr<File_I> m_file;

	std::shared_ptr<void> m_mmap;
	off_t m_offset = 0;

	UnitInternals m_internals;

public:
	Unit(std::shared_ptr<File_I> file, UnitInternals internals, std::shared_ptr<void> memMap, off_t mapOffset);
	Unit(std::shared_ptr<File_I> file);
	~Unit();

	std::shared_ptr<File_I> getFile() { return m_file; }
	void syncMe() noexcept;

	Marshal_t marshal() const;

	void setProcessed(bool val);
	bool isProcessed() const { return m_internals.isProcessed; }
};
