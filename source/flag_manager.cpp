#include "flag_manager.h"
#include "flag_list.h"
#include <iostream>
#include "fl2_utils.h"

FlagManager::FlagManager()
{
    head = tail = new FlagList{'-', "nonflags", false};
    len = 1;
}

void FlagManager::cleanup()
{
	FlagList *bin{head};
	FlagList *nxt{head -> next};

	for (int n{0}; n < len - 1; n++)
	{
		delete bin;
		bin = nxt;
		nxt = nxt -> next;
	}
	delete bin;
	head = tail = nullptr;
}

FlagManager::~FlagManager()
{
	cleanup();
	head = tail = nullptr;
}

void FlagManager::SetFlag(char ipName, bool ipArgsExpected)
{
	FlagList *swp = new FlagList{ipName, ipArgsExpected};
	tail -> next = swp;
	tail = swp;
	len++;
}

void FlagManager::SetFlag(const char *ipAlias, bool ipArgsExpected)
{
	FlagList *swp = new FlagList{ipAlias, ipArgsExpected};
	tail -> next = swp;
	tail = swp;
	len++;
}


void FlagManager::SetFlag(char ipName, const char *ipAlias, bool ipArgsExpected)
{
	FlagList *swp = new FlagList{ipName, ipAlias, ipArgsExpected};
	tail -> next = swp;
	tail = swp;
	len++;
}

FlagList* FlagManager::operator[] (int index)
{
	if (index == 0)
	{
		return head;
	}
	else if (index >= len - 1)
	{
		return tail;
	}
	else
	{
		FlagList *op{head};
		for (int n{0}; n < index; n++)
		{
			op = op -> next;
		}
		return op;
	}
}

FlagList* FlagManager::FetchFlag(char ipName)
{
    FlagList *x{head};
    while (x)
    {
        if (x->name == ipName)
        {
            return x;
        }
        x = x->next;
    }
    return head;
}

FlagList* FlagManager::FetchFlag(const char *ipAlias)
{
    FlagList *x{head};
    int ipLen{fl2_utils::len(ipAlias)};
    while (x)
    {
        if(x -> alias)
        {
            if (ipLen == fl2_utils::len(x->alias))
            {
                bool match{true};
                for (int n{0} ; n < ipLen ; n++)
                {
                    if (ipAlias[n] != (x->alias)[n])
                    {
                        match = false;
                    }
                }
                if (match)
                {
                    return x;
                }
            }
        }
        x = x->next;
    }
    return head;
}


void FlagManager::Init(int argc, char **argv)
{
    for (int argIndex{1}; argIndex < argc ; argIndex++)
    {
        int argLen{fl2_utils::len(argv[argIndex])};

        if (argv[argIndex][0] == '-' && argLen > 1 /*&& (fl2_utils::IsAlpha(argv[argIndex][1]))*/)
        {
            if (argLen == 2)
            {
                FlagList *y{FetchFlag(argv[argIndex][1])};

                if (y->name == '-')
                {
                    throw "Flag not recognised";
                }

                y->raised = true;
                
                if (y->argsExpected)
                {
                    if (argIndex < argc - 1 && argv[argIndex + 1][0] != '-')
                    {
                        y->Append(argv[argIndex + 1]);
                        argIndex++;
                    }
                    else
                    {
                        throw "1 or more arguments expected for a flag but 0 given";
                    }
                }
            }
            else if (argLen > 2)
            {
                if(argv[argIndex][1] == '-')
                {
                    char *query{fl2_utils::stripDashes(argv[argIndex])};
                    FlagList *y{FetchFlag(query)};
                    
                    if (y->name == '-')
                    {
                        throw "Flag not recognised";
                    }

                    y->raised = true;

                    if (y->argsExpected)
                    {
                        if (argIndex < argc - 1 && argv[argIndex + 1][0] != '-')
                        {
                            y->Append(argv[argIndex + 1]);
                            argIndex++;
                        }
                        else
                        {
                            throw "1 or more arguments expected for a flag but 0 given";
                        }
                    }

                    delete[] query;
                    query = nullptr;
                }
                else
                {
                    for (int n{1} ; n < argLen ; n++)
                    {
                        FlagList *y{FetchFlag(argv[argIndex][n])};
                        
                        if (y->argsExpected)
                        {
                            throw "Flags that take arguments cannot be melded with others";
                        }

                        y->raised = true;
                    }
                }
            }
        }
        else
        {
            head->Append(argv[argIndex]);
            if (!head->raised)
            {
                head->raised = true;
            }
        }
    }
}

bool FlagManager::IsRaised(char query)
{
    FlagList *y{FetchFlag(query)};
    return y->raised;
}

bool FlagManager::IsRaised(const char *query)
{
    FlagList *y{FetchFlag(query)};
    return y->raised;
}

int FlagManager::ArgCount(char query)
{
    FlagList *y{FetchFlag(query)};
    return y->Len();
}

int FlagManager::ArgCount(const char *query)
{
    FlagList *y{FetchFlag(query)};
    return y->Len();
}

char* FlagManager::FetchArg(char query, int index)
{
    FlagList *y{FetchFlag(query)};
    if (y->Len() < 1 || index > y->Len() || index < 0)
    {
        return fl2_utils::deepCopyString("");
    }
    else
    {
        return y->operator[](index);
    }

}

char* FlagManager::FetchArg(const char *query, int index)
{
    FlagList *y{FetchFlag(query)};
    if (y->Len() < 1 || index > y->Len() || index < 0)
    {
        return fl2_utils::deepCopyString("");
    }
    else
    {
        return y->operator[](index);
    }
}
