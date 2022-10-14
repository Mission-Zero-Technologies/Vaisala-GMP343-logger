#ifndef FLAG_LIST
#define FLAG_LIST

#include "flag_node.h"

class FlagList
{
	public:
		char name;
		char *alias;
		FlagList *next;
		bool raised, argsExpected;
		FlagList(char ipName, bool ipArgsExpected);
		FlagList(const char *ipAlias, bool ipArgsExpected);
		FlagList(char ipName, const char *ipAlias, bool ipArgsExpected);
		~FlagList();
		void Append(const char *ipValue);
		int Len();
		char* operator[] (int index);
	
	private:
		FlagNode *head;
		FlagNode *tail;
		int len;
		void cleanup();
};

#endif
