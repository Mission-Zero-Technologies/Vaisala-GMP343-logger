#ifndef FLAG_NODE
#define FLAG_NODE

class FlagNode
{
	public:
		char *value;
		FlagNode *next;
		FlagNode(const char *ipValue);
		~FlagNode();
};

#endif
