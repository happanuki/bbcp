#pragma once

#include <sys/types.h>

#include <string>
#include <memory>
#include <vector>


struct Marshal_t {
	std::shared_ptr<char> ptr;
	uint64_t size;

	Marshal_t operator+(const Marshal_t&);
};

struct Offset
{
	off_t offset;
	int size;

	bool operator==(const Offset& rhs) { return (offset == rhs.offset) && (size == rhs.size); }
	bool operator!=(const Offset& rhs) { return !operator==(rhs); }
};


using Offsets_t = std::vector< Offset > ; // partition-dev-file-offset, and its status


struct FileInternals
{
	std::string fileName;
	Offsets_t offsets;
};


struct UnitInternals
{
	bool isProcessed = false;
};


enum class AppMode_E
{
	APP_START,
	APP_CONTINUE,
	APP_BAD_MODE
};

struct AppParameters
{
	std::string appName;

	std::string diskDev;
	std::string partDev;
	std::string storageFile = "storage.file";
	std::string restoreDir;

	AppMode_E mode = AppMode_E::APP_BAD_MODE;
};
