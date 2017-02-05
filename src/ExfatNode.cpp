#include "sys/stat.h"

#include "ExfatNode.h"
#include "Exception.h"

ExfatNode::ExfatNode(std::shared_ptr<exfat> exfatHandler,exfat_node* node) :
	m_node(node),
	m_exfat(exfatHandler)
{
}


ExfatNode::~ExfatNode()
{
	close();
}


int ExfatNode::getClusterSize() const
{
	return CLUSTER_SIZE( *m_exfat->sb );
}



ExfatNodeTypeE ExfatNode::getType() const noexcept
{
	struct stat l_st;
	exfat_stat(m_exfat.get(), m_node, &l_st);

	switch (l_st.st_mode & S_IFMT) {

	case S_IFDIR: return ExfatNodeTypeE::EXF_NODE_DIR;
	case S_IFREG: return ExfatNodeTypeE::EXF_NODE_FILE;

	default: return ExfatNodeTypeE::EXF_NODE_UNKNOWN;
	}
}


std::string ExfatNode::getName() const
{
	std::string curNodeName(EXFAT_NAME_MAX+1, 0);
	auto b = const_cast<char*>( curNodeName.data());
	exfat_get_name( m_node, b, EXFAT_NAME_MAX);
	return curNodeName.c_str();
}


off_t ExfatNode::getSize() const
{
	return m_node->size;
}


exfat_iterator& ExfatNode::open() throw (std::exception&)
{
	if ( m_isOpen) {
		return m_it;
	}

	if ( exfat_opendir(m_exfat.get(), m_node, &m_it)) {
		THROW_SYS_EXCEPTION("exfat_opendir failed");
	}

	m_isOpen = true;
	return m_it;
}


void ExfatNode::close()
{
	if ( m_isOpen) {
		exfat_closedir( m_exfat.get(), &m_it);
	}
	m_isOpen = false;
}


uint64_t ExfatNode::_calcNumClusters() const
{
	auto nsz = getSize();
	auto csz = getClusterSize();
	return (nsz % csz) ? nsz/csz + 1: nsz/csz ;
}

Offsets_t ExfatNode::getOffsets() const
{
	auto calc_clusters = _calcNumClusters() - 1; // -1 for first cluster

	auto next = m_node->start_cluster;
	Offsets_t out;

	Offset val = { exfat_c2o(m_exfat.get(),next), getClusterSize() };
	out.push_back(val);


	bool badMark = false;
	for (auto i=0; i < calc_clusters; ++i ) {

		if (badMark) {
			Offset val = { 0 };
			out.push_back(val);
			continue;
		} //loop to end with 0

		next = exfat_next_cluster( m_exfat.get(), m_node, next);
		if ( next == EXFAT_CLUSTER_BAD) {
			//TODO: there should be errno analysis
			Offset val = { 0 };
			out.push_back(val);
			badMark=true;
			continue;
		}

		Offset val = { exfat_c2o(m_exfat.get(),next), getClusterSize() };
		out.push_back(val);
	}

	/* last size fix, its can be != getClusterSize() */
	auto lastBytes = getSize() % getClusterSize() ;
	if ( lastBytes ) {
		out[calc_clusters].size = lastBytes ;
	}

	return out;
}
