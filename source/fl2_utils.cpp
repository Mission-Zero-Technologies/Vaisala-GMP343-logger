#include "fl2_utils.h"

int fl2_utils::len(const char *ip)
{
	int op{0};
	while (ip[op] != '\0')
	{
		op++;
	}
	return op;
}

char* fl2_utils::deepCopyString(const char *ip)
{
	int opLen{fl2_utils::len(ip)};
	char *op{new char[opLen + 1]};
	for (int n{0}; n < opLen; n++)
	{
		op[n] = ip[n];
	}
	op[opLen] = '\0';
	return op;
}

char* fl2_utils::stripDashes(const char *ip)
{
	int opLen{fl2_utils::len(ip) - 2};
	char *op{new char[opLen + 1]};
	for (int n{0}; n < opLen; n++)
	{
		op[n] = ip[n + 2];
	}
	op[opLen] = '\0';
	return op;
}

int fl2_utils::IsAlpha(char ip)
{
	int x{(int)ip};

	if ((x > 64 && x < 91) || (x > 96 && x < 123))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
