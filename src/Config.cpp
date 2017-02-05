#include "Config.h"

Config ConfigFactory::getConfig()
{
	Config out = {
			{ "nomerges"	, "2" } , // no merges bio
			{"read_ahead_kb", "0"} ,
			{"scheduler"	, "noop"} ,
			{"add_random"	, "0"} ,
	};

	return out;
}
