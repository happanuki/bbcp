#pragma once

#include "Types.h"

#include "interfaces/File_I.h"

namespace Marshal
{
	Marshal_t marshalString( std::string);
	std::string unmarshalString( char** input ); //*input will be moved to point next elem;

	Marshal_t marshalBool( bool);
	bool unmarshalBool( char** input ); //*input will be moved to point next elem;

	Marshal_t marshalOffsets( const Offsets_t& );
	Offsets_t unmarshalOffsets( char** input ); //*input will be moved to point next elem;


	Marshal_t marshalFileInternals( const FileInternals& );
	FileInternals unmarshalFileInternals( char** input);

	Marshal_t marshalUnitInternals( const UnitInternals& );
	UnitInternals unmarshalUnitInternals( char** input );

}
