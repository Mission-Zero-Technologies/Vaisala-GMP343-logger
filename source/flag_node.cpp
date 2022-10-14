#include "flag_node.h"
#include "fl2_utils.h"

FlagNode::FlagNode(const char *ipValue)
{
	int ipLen{fl2_utils::len(ipValue)};
	value = new char[ipLen + 1];
	for (int n{0}; n < ipLen ; n++)
	{
		value[n] = ipValue[n];
	}
	value[ipLen] = '\0';
	
	next = nullptr;
}

FlagNode::~FlagNode()
{
	delete[] value;
	value = nullptr;
}
