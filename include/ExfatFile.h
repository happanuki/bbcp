#pragma once

#include "exfat.h"

#include "interfaces/File_I.h"

class ExfatFile : public File_I
{
	FileInternals m_internals;

public:
	ExfatFile() = default;
	ExfatFile(FileInternals fi); // c++11 rvalue construction

	~ExfatFile() {}

	std::string getName() override { return m_internals.fileName; }
	Offsets_t getOffsets() override { return m_internals.offsets; }

	Marshal_t marshal() override;
	FileInternals& getInternals() { return m_internals; }
};
