#include <cstring>

#include "Types.h"


Marshal_t Marshal_t::operator+(const Marshal_t& rhs)
{
	auto totalSize = rhs.size + size;

	std::shared_ptr<char> resultingBuf( new char[totalSize], std::default_delete<char[]>());

	memcpy(resultingBuf.get(), ptr.get(), size);
	memcpy(resultingBuf.get() + size, rhs.ptr.get(), rhs.size);

	Marshal_t out;
	out.ptr = resultingBuf;
	out.size = totalSize;

	return out;
}
