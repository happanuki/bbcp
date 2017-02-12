#include "sys/mman.h"
#include "unistd.h"

#include "Syscalls.h"
#include "Exception.h"

#include "UnitStorage.h"
#include "ExfatFile.h"
#include "Marshal.h"

UnitStorage::UnitStorage(std::string filename) :
	m_storeFile(filename)
{

}

void UnitStorage::init()
{
	m_mmap = _initMmap();
}

std::shared_ptr<void> UnitStorage::_initMmap() throw (std::exception&)
{
	auto fd = System::open(m_storeFile.c_str(), O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

	struct stat st = System::getFileStat(m_storeFile);

	m_storeFileSize = st.st_size;

    void* ptr = mmap(NULL, m_storeFileSize, PROT_READ| PROT_WRITE, MAP_SHARED, fd, 0);

    std::shared_ptr<void> out(ptr,[fd,this](void* in) {
    	if (in != MAP_FAILED) {
    		munmap(in,m_storeFileSize);
    	}
    	close(fd);
    });

    if (ptr == MAP_FAILED ) {
    	THROW_SYS_EXCEPTION("mmap failed");
    }

    return out;
}

void UnitStorage::store(const std::vector<Unit>& in)
{
	auto fd = System::openAutoClose(m_storeFile.c_str(), O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	for (auto& it: in) {
		auto m = it.marshal();
		System::write(*fd,m.ptr.get(),m.size);
	}
}

std::vector<Unit> UnitStorage::load()
{
	if (! isInit()) {
		init();
	}

	uint64_t off = 0;
	std::vector<Unit> out;

	while( (m_storeFileSize - off) > 0 ) {

		char* ptr = (char*) m_mmap.get() + off;
		auto ptrInitial = ptr;

		FileInternals_t fi = Marshal::unmarshalFileInternals(&ptr);

		auto f = std::make_shared<ExfatFile>( ExfatFile(fi));

		UnitInternals ui = Marshal::unmarshalUnitInternals(&ptr);

		out.emplace_back( Unit(f,ui, m_mmap, off));

		off += (ptr - ptrInitial);
	}

	return out;
}
