#pragma once

#include "Types.h"

class File_I
{
public:
	File_I() {}
	virtual ~File_I() {}

	virtual std::string getName() = 0;
	virtual Offsets_t getOffsets() = 0;
	virtual Marshal_t marshal() = 0;
};
