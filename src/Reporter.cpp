#include <sstream>

#include <Syscalls.h>

#include "Reporter.h"


static std::string prettyStringPrint(const std::string& str)
{
	auto window = System::getTerminalSize();

	std::stringstream s;

	if ( str.size() > (window.ws_col / 2) ) {
		auto pos1 = str.rbegin() + window.ws_col / 2;
		std::string modStr(pos1.base(), str.end());
		modStr = "..." + modStr;
		s << modStr;
	}
	else {
		s << str;
	}

	return s.str();
}

static void prettyDirPrint(const std::vector<std::string>& dirs, std::ostream& s, size_t& initialColumn)
{
	auto window = System::getTerminalSize();

	for (const auto& d: dirs) {
		s << "* " << d << "\n";

		if (! (++initialColumn % (window.ws_col-2) )) {
			s << "press enter to continue\n" ;
		}

	}
}

static void prettyFilePrint(const std::vector<ReportFile_t>& files, std::ostream& s, size_t& initialColumn)
{
	auto window = System::getTerminalSize();

	for (const auto& f: files) {

		s << "* " << prettyStringPrint(f.fileName) << " " << f.totalSize / 1024 << "K" << f.totalSize % 1024 << "Bytes" ;

		if (f.badOffset) {
			s << "\tBad Offset : " << std::hex << f.badOffset << std::dec;
		}

		s << "\n";

		if (! (++initialColumn % (window.ws_col-2) )) {
			s << "press enter to continue\n" ;
		}
	}
}



Reporter::Reporter()
{
	startNow();
}


void Reporter::addLostDir( std::string dir)
{
	m_lostDirs.push_back( dir);
}


void Reporter::addBadFile( ReportFile_t file)
{
	m_badFiles.push_back(file);
}


void Reporter::addGoodFile( ReportFile_t file)
{
	m_goodFiles.push_back( file);
}

void Reporter::printReport()
{
	std::ostream& s = std::cout;
	auto stopDate = System::time(nullptr);

	auto deltaTime = m_startDate - stopDate;
	auto hrs = deltaTime / 3600 ;
	auto mins = ( deltaTime % 3600 ) / 60 ;
	auto secs = ( deltaTime % 3600 ) % 60 ;

	printf("\e[2J\e[H"); // clear screen

	s	<< "\t\t----> Report <----- \n"
		<< "\tSTART TIME : " << System::timeStr(&m_startDate) << "\n"
		<< "\tSTOP  TIME : " << System::timeStr( &stopDate) << "\n"
		<< "\tWORK  TIME : " << hrs << ":" << mins << ":" << secs << "  [ hrs:mins:secs ]" << "\n\n"
		<< "\t\t----> Unrecoverable dirs <------ \n";

	size_t curCol=7;
	prettyDirPrint(m_lostDirs,s,curCol);

	s	<< "\t\t----> Fully restored files <------ \n";
	prettyFilePrint(m_goodFiles,s,curCol);

	s	<< "\t\t----> Error files <------ \n";
	prettyFilePrint(m_badFiles,s,curCol);

}


void Reporter::startNow()
{
	m_startDate = System::time(nullptr); // Current time
}
