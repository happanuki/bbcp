#include <fstream>

#include <Logger.h>
#include <Syscalls.h>
#include <Exception.h>

#include "Application.h"
#include "ArgsParser.h"


int main(int argc, char** argv, char** envp)
{

	ArgsParser parser( argc, argv, envp);

	Application app( parser.get());

	app.init();

	app.run();

	return 0;
}
