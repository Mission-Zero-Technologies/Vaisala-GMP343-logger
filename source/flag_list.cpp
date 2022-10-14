#include "flag_list.h"
#include "flag_node.h"
#include "fl2_utils.h"
#include <iostream>

FlagList::FlagList(char ipName, bool ipArgsExpected)
{
	name = ipName;
	alias = nullptr;
	head = tail = nullptr;
	len = 0;
	raised = false;
	argsExpected = ipArgsExpected;
}

FlagList::FlagList(const char *ipAlias, bool ipArgsExpected)
{
	alias = fl2_utils::deepCopyString(ipAlias);
	head = tail = nullptr;
	len = 0;
	raised = false;
	argsExpected = ipArgsExpected;
}

FlagList::FlagList(char ipName, const char *ipAlias, bool ipArgsExpected)
{
	name = ipName;
	alias = fl2_utils::deepCopyString(ipAlias);
	head = tail = nullptr;
	len = 0;
	raised = false;
	argsExpected = ipArgsExpected;
}

void FlagList::cleanup()
{
	FlagNode *bin{head};
	FlagNode *nxt{head -> next};

	for (int n{0}; n < len - 1; n++)
	{
		delete bin;
		bin = nxt;
		nxt = nxt -> next;
	}
	delete bin;
	head = tail = nullptr;
}

FlagList::~FlagList()
{
	if (alias)
	{
		delete[] alias;
	}

	if (len > 0)
	{
		cleanup();
		head = tail = nullptr;
	}
}

void FlagList::Append(const char *ipValue)
{
	FlagNode *swp = new FlagNode{ipValue};
	if (len == 0)
	{
		head = tail = swp;
	}
	else
	{
		tail -> next = swp;
		tail = swp;
	}
	len++;
}

int FlagList::Len(){return len;}

char* FlagList::operator[] (int index)
{
	if (len == 0)
	{
		return fl2_utils::deepCopyString("Error");
	}
	else if (index == 0)
	{
		return fl2_utils::deepCopyString(head->value);
	}
	else if (index >= len - 1)
	{
		return fl2_utils::deepCopyString(tail->value);
	}
	else
	{
		FlagNode *op{head};
		for (int n{0}; n < index; n++)
		{
			op = op->next;
		}
		return fl2_utils::deepCopyString(op->value);
	}
}