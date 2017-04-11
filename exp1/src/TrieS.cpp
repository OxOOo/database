#include "TrieS.h"

#include <cstring>
#include <iostream>

using namespace std;

TrieS::TrieS()
{
    pool = new int[BUFFER_SIZE*128+BUFFER_SIZE];
    son = new int*[BUFFER_SIZE];
    for(int i = 0; i < BUFFER_SIZE; i ++)
        son[i] = pool + i*128;
    flag = pool + BUFFER_SIZE*128;
}

TrieS::~TrieS()
{
    delete[] pool;
    delete[] son;
}

// only clear the each_pool
void TrieS::init()
{
    cnt = 1;
    root = newNode();
}

// return 1 if not exist
int TrieS::insert(const char* ptr, int length)
{
    int pos = root;
    while(length --)
    {
        if (!son[pos][(int)*ptr]) {
            son[pos][(int)*ptr] = newNode();
        }
        pos = son[pos][(int)*ptr];
        ptr ++;
    }
    int ret = flag[pos];
    flag[pos] = 1;
    return ret^1;
}

// return 1 if exist
int TrieS::exist(const char* ptr, int length) const
{
    int pos = root;
    while(length -- && pos)
    {
        pos = son[pos][(int)*ptr];
        ptr ++;
    }
    return pos && flag[pos];
}

int TrieS::newNode()
{
    int op = cnt ++;
    memset(son[op], 0, sizeof(int)*128);
    flag[op] = 0;
    return op;
}