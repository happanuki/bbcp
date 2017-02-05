#include <Syscalls.h>
#include <Logger.h>

#include <cstring>

#include "Unit.h"
#include "Marshal.h"


Unit::Unit(std::shared_ptr<File_I> file, UnitInternals internals, std::shared_ptr<void> memMap, off_t mapOffset) :
	m_file(file),
	m_mmap(memMap),
	m_offset(mapOffset),
	m_internals(internals)
{
}

Unit::Unit(std::shared_ptr<File_I> file) :
		m_file(file)
{
}


Unit::~Unit()
{
}


void Unit::syncMe() noexcept
{
	auto ptr = (char*)m_mmap.get() + m_offset;
	auto m = marshal();
	memcpy(ptr,m.ptr.get(),m.size);
}


Marshal_t Unit::marshal() const
{
	auto filem = m_file->marshal();
	auto intm = Marshal::marshalUnitInternals( m_internals);

	return filem + intm;
}

void Unit::setProcessed(bool val)
{
	m_internals.isProcessed = val;
	syncMe();
}
