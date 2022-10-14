#ifndef FLAG_MANAGER
#define FLAG_MANAGER

#include "flag_list.h"

class FlagManager
{
    public:
        FlagManager();
        ~FlagManager();
        void SetFlag(char ipName, bool ipArgsExpected);
        void SetFlag(const char *ipAlias, bool ipArgsExpected);
        void SetFlag(char ipName, const char *ipAlias, bool ipArgsExpected);
        void Init(int argc, char **argv);
        bool IsRaised(char query);
        bool IsRaised(const char *query);
        int ArgCount(char query);
        int ArgCount(const char *query);
        char* FetchArg(char query, int index);
        char* FetchArg(const char *query, int index);
        
    
    private:
        FlagList *head;
        FlagList *tail;
        //FlagList nonflags{'-', "nonflags", false};
        int len;
        void cleanup();
        FlagList* operator[] (int index);
        FlagList* FetchFlag(char ipName);
        FlagList* FetchFlag(const char *ipAlias);
};

#endif
