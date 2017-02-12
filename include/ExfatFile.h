#pragma once

#include "exfat.h"

#include "interfaces/File_I.h"

class ExfatFile : public File_I
{
	FileInternals_t m_internals;

public:
	ExfatFile() = default;
	ExfatFile(FileInternals_t fi); // c++11 rvalue construction

	~ExfatFile() {}

	std::string getName() override { return m_internals.fileName; }
	Offsets_t getOffsets() override { return m_internals.offsets; }

	Marshal_t marshal() override;
	FileInternals_t& getInternals() { return m_internals; }
};
