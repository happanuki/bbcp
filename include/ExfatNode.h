#pragma once

#include "sys/types.h"
#include "exfat.h"

#include <string>
#include <exception>
#include <memory>

#include "interfaces/File_I.h"


enum ExfatNodeTypeE
{
	EXF_NODE_FILE,
	EXF_NODE_DIR,
	EXF_NODE_UNKNOWN
};

class ExfatNode
{
	exfat_node* m_node;
	std::shared_ptr<exfat> m_exfat;
	exfat_iterator m_it = { 0 };

	bool m_isOpen = false;

	uint64_t _calcNumClusters() const;

public:
	ExfatNode(std::shared_ptr<exfat> exfatHandler, exfat_node* node);
	~ExfatNode();

	std::string getName() const;
	ExfatNodeTypeE getType() const noexcept;
	off_t getSize() const;

	exfat_iterator& open() throw (std::exception&);
	void close();

	exfat_node* get() const { return m_node; }

	Offsets_t getOffsets() const;
	int getClusterSize() const ;
};
