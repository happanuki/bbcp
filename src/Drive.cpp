#include <unistd.h>

#include "Drive.h"
#include "Syscalls.h" //CppGenerics
#include "Exception.h"
#include "Logger.h"

Drive::Drive(const std::string& devName) :
	m_devName(devName)
{
}


Drive::~Drive()
{
	uninit();
}

int Drive::adoptiveReadWrite(int fd_read, int fd_write, off_t offset, int min_bytes, int good_bytes)
{
	auto buf = System::getBufferRAI_nt(good_bytes,true);

	if ( good_bytes % min_bytes ) {
		errno = EINVAL;
		return -1;
	}

	auto ka = good_bytes / min_bytes;

	do {
		errno = 0;
		auto cur_bytes = ka * min_bytes ;

		auto ret = read(fd_read,buf.get(),cur_bytes);
		if ( ret == -1 ) {
			if ( errno == EIO) {
				--ka;
			}
			else {
				return -1;
			}
		}
		else if ( ret == cur_bytes ) return ret;

	} while (ka);
}



static std::string l_readFile(const std::string& fileName)
{
	auto fd = System::openAutoClose(fileName.c_str(),O_RDONLY,0);

	auto sz = sysconf(_SC_PAGESIZE);

	std::string out(sz,0);

	char* buf = const_cast<char*>( out.data()); //!!

	System::read(*fd,buf,sz);

	return out;
}


static void l_writeFile(const std::string& fileName, const std::string& val)
{
	auto fd = System::openAutoClose(fileName.c_str(),O_WRONLY,0);
	System::write(*fd,val.c_str(), val.size());
}


void Drive::_setSelfParameters()
{
	std::string out = l_readFile("/sys/class/block/" + m_devName + "/queue/minimum_io_size");
	m_minIO_sz = std::stoull(out, nullptr, 10);

	out = l_readFile("/sys/class/block/" + m_devName + "/queue/hw_sector_size");
	m_sector_sz = std::stoull(out, nullptr, 10);
}


void Drive::_applyConfig(const Config& c)
{

	for (auto& it: c ) {
		auto path = "/sys/class/block/" + m_devName + "/queue/" + it.first;
		l_writeFile( path, it.second);
	}
}


void Drive::init() throw (std::exception&)
{
	_applyConfig( ConfigFactory::getConfig());
	_setSelfParameters();

	m_isRdy = true;
}


void Drive::uninit() noexcept
{
	//TODO: add all parameters
	m_isRdy = false;

}


off_t Drive::getSize()
{
	auto strSize = l_readFile("/sys/class/block/" + m_devName + "/size");

	return std::stoull(strSize);
}
