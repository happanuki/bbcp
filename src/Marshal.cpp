#include "Marshal.h"

#include <cstring>

namespace Marshal {


Marshal_t marshalString(std::string what)
{
	Marshal_t ret;

	ret.size = what.size() + 1;
	ret.ptr = std::shared_ptr<char>(new char[ret.size], std::default_delete<char[]>() );

	strcpy(ret.ptr.get(), what.c_str());

	return ret;
}


std::string unmarshalString( char** input )
{
	std::string out(*input);
	*input += out.size() + 1;
	return out;
}


Marshal_t marshalBool( bool what)
{
	Marshal_t ret;

	ret.size = sizeof(what);
	ret.ptr = std::shared_ptr<char>(new char[ret.size], std::default_delete<char[]>());

	memcpy(ret.ptr.get(), &what, ret.size);

	return ret;
}


bool unmarshalBool( char** input )
{
	bool* ptr = reinterpret_cast<bool*>(*input);
	*input += sizeof(bool);
	return *ptr;
}


Marshal_t marshalOffsets( const Offsets_t& in)
{
	using ElemType = Offsets_t::value_type;

	Marshal_t ret;

	auto elemSize = sizeof(ElemType);
	auto numElems = in.size();

	ret.size = sizeof(size_t) + numElems * elemSize;

	ret.ptr = std::shared_ptr<char>(new char[ret.size], std::default_delete<char[]>());

	//writing size
	auto ptr = reinterpret_cast<size_t*>( ret.ptr.get());
	*ptr++ = numElems;

	//writing vector
	auto eptr = reinterpret_cast<ElemType*>(ptr);

	for (auto& it: in) {
		*eptr++ = it;
	}

	return ret;
}


Offsets_t unmarshalOffsets( char** input )
{
	char* ptr = *input;

	using ElemType = Offsets_t::value_type ;
	auto elemSize = sizeof(ElemType);

	//reading vector size
	auto sptr = reinterpret_cast<size_t*>(ptr);
	auto vecSize = *sptr;
	++sptr;

	//reading vector
	Offsets_t out(vecSize);

	auto eptr = reinterpret_cast<ElemType*>(sptr);

	for (auto i = 0; i < vecSize; ++i ) {
		out[i] = *eptr;
		++eptr;
	}

	ptr = reinterpret_cast<char*>(eptr);
	*input = ptr;

	return out;
}


Marshal_t marshalFileInternals( const FileInternals& in)
{
	auto m1 = marshalString(in.fileName);
	auto m2 = marshalOffsets(in.offsets);

	auto out = m1 + m2;

	return out;
}


FileInternals unmarshalFileInternals( char** input)
{
	auto str = unmarshalString( input);
	auto ofs = unmarshalOffsets( input);

	FileInternals ret;

	ret.fileName = str;
	ret.offsets = ofs;

	return ret;
}


Marshal_t marshalUnitInternals( const UnitInternals& in)
{
	return marshalBool(in.isProcessed);
}


UnitInternals unmarshalUnitInternals( char** input )
{
	UnitInternals out;

	out.isProcessed = unmarshalBool(input);

	return out;

}


} //namespace
