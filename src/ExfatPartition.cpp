#include <sys/sysmacros.h>
#include <unistd.h>

#include "Syscalls.h"
#include "Exception.h"
#include <Logger.h>

#include "ExfatNode.h"
#include "ExfatPartition.h"

// C++11 style, r-val construction
ExfatPartition::ExfatPartition( std::string devFileName) :
	m_devFileName(devFileName)
{
}


void ExfatPartition::init() throw(std::exception&)
{
	std::lock_guard<std::mutex> g(m_mtx);

	if ( isInit()) {
		THROW_LOGIC_EXCEPTION("re-init attempt");
	}

	m_sectorOffset = _getSectorOffset();

	exfat* p = new exfat;

	auto ret = exfat_mount(p, m_devFileName.c_str(), m_mountOpts.c_str());
	if ( ret ) {
		delete p;
		THROW_SYS_EXCEPTION("exfat mount failed");
	}

	m_exfat.reset ( p , [](exfat* ef) {
		exfat_unmount( ef);
		delete ef;
	});

	m_cluster_size = CLUSTER_SIZE(*m_exfat->sb);

	m_init = true;
}


off_t ExfatPartition::_getSectorOffset() throw (std::exception&)
{
	auto st = System::getFileStat( m_devFileName);
	auto mjor = std::to_string( major(st.st_rdev));
	auto mnor = std::to_string( minor(st.st_rdev));

	std::string path = std::string("/sys/dev/block/") + mjor + ":" + mnor +"/start";

	if ( System::isFileExist(path)) {

		auto fd = System::openAutoClose( path.c_str(), O_RDONLY);

		auto sz = sysconf(_SC_PAGESIZE);

		std::string buf(sz,0);
		char* buf_p = const_cast<char*>( buf.data());

		System::read( *fd, buf_p, sz);

		return std::stoull(buf_p);
	}
	else {
		return 0;
	}
}

void ExfatPartition::_dirsRecursiveWalk(exfat_node* startNode, std::string curPath, std::vector<ExfatFile>& out)
{

	ExfatNode node(m_exfat, startNode);

	switch ( node.getType()) {

	case ExfatNodeTypeE::EXF_NODE_FILE : {

		FileInternals_t fi;
		fi.fileName = curPath + node.getName();
		fi.offsets = node.getOffsets();

		ExfatFile f( std::move(fi));
		out.push_back(f);
		break;
	}

	case ExfatNodeTypeE::EXF_NODE_DIR : {

		auto it = node.open(); // close will be called on node's destruction

		exfat_node* l_n = nullptr;

		while( (l_n = exfat_readdir( m_exfat.get(), &it)) != nullptr ) {
			_dirsRecursiveWalk(l_n, curPath + node.getName() + "/", out);
			exfat_put_node(m_exfat.get(), l_n);
		}

		break;
	}

	default: {
		THROW_LOGIC_EXCEPTION("Unknown node type")
	}

	} //switch
}

std::vector<ExfatFile> ExfatPartition::getFileList()
{
	std::vector<ExfatFile> out;
	_dirsRecursiveWalk(m_exfat->root,"",out);

	return out;
}

std::vector<Unit> ExfatPartition::getUnitList()
{
	std::vector<Unit> out;

	for (auto& it: getFileList()) {
		auto f = std::make_shared<ExfatFile>( ExfatFile(it));
		out.emplace_back( Unit(f));
	}

	return out;
}


//std::vector<ExfatDir> ExfatPartition::getDirList()
//{
//	ExfatDir dir(m_exfat,m_exfat->root);
//	dir.open();
//	return dir.getDirs();
//}
/*
 *
//
//	if ( node.getType() == ExfatNodeTypeE::EXF_NODE_FILE)
//
//	do {
//		ExfatNode node(m_exfat,cur);
//
//		if (cur->child) {
//			//directory
//			curPath += std::string(curNodeName.c_str()) + "/";
//			_dirsRecursiveWalk(cur->child, curPath, out);
//		}
//		else {
//			//file
//			ExfatFile f(curPath + curNodeName, std::vector<Offset>() );
//			out.push_back(f);
//		}
//		cur = cur->next;
//	}
//	while (it != nullptr );
//
//	exfat_closedir( m_exfat.get(),&it);
 */
