#ifndef TRIE_S_H
#define TRIE_S_H

#include "common.h"

class TrieS
{
public:
    TrieS();
    ~TrieS();

    void init();
    // return 1 if not exist
    int insert(const char* ptr, int length);
    // return 1 if exist
    int exist(const char* ptr, int length) const;

private:
    int *pool;
    int **son;
    int *flag;
    int root, cnt;

    int newNode();
};

#endif // TRIE_S_H