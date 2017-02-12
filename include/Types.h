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

struct Offset_t
{
	off_t offset;
	int size;

	bool operator==(const Offset_t& rhs) { return (offset == rhs.offset) && (size == rhs.size); }
	bool operator!=(const Offset_t& rhs) { return !operator==(rhs); }
};


using Offsets_t = std::vector< Offset_t > ; // partition-dev-file-offset, and its status


struct FileInternals_t
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

enum class RestoreMode_E
{
	RESTORE_ONLY_GOOD,
	RESTORE_ALL,
	RESTORE_UNKNOWN
};


struct AppParameters_t
{
	std::string appName;

	std::string diskDev;
	std::string partDev;
	std::string storageFile = "storage.file";
	std::string restoreDir;

	AppMode_E app_mode = AppMode_E::APP_BAD_MODE;
	RestoreMode_E restore_mode = RestoreMode_E::RESTORE_ONLY_GOOD;
};


struct UdevEventDescriptor_t
{
	std::string action; // either add or remove
	std::string devType; // disk, partition, ...
	std::string subSystem; // "block" , @ sys/class/...
};

