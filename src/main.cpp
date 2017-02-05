#include <fstream>

#include <Logger.h>
#include <Syscalls.h>
#include <Exception.h>

#include "ExfatPartition.h"
#include "UnitStorage.h"
#include "Processor.h"
#include "Application.h"


int main(int argc, char** argv)
{
	Application app(argc,argv);

	app.init();

	app.run();

	return 0;
}
