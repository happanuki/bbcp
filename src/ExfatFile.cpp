
#include "ExfatFile.h"
#include "Marshal.h"

ExfatFile::ExfatFile(FileInternals_t fi) :
	File_I(),
	m_internals( std::move(fi))
{
}


Marshal_t ExfatFile::marshal()
{
	return Marshal::marshalFileInternals(m_internals);
}

