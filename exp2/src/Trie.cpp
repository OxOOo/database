#include "Trie.h"

using namespace std;

Trie::Node::Node()
{
    memset(son, 0, sizeof(son));
}

Trie::Trie()
{
    each_pool = NULL;
    each_tail = 0;
    root = newNode();
}

Trie::~Trie()
{
    for(auto p: pools)
        delete[] p;
    pools.clear();
}

void Trie::adjust()
{
    for(auto p: pools)
        for(int k = 0; k < EACH_ALLOC; k ++)
        {
            if (p[k].vec.size() > 0)
            {
                int size = p[k].vec.size();
                p[k].ds = p[k].vec.data();
                p[k].de = p[k].ds + size;
            } else p[k].ds = p[k].de = NULL;
        }
}

Trie::Node* Trie::insert(const int& d, const char* ptr, int length)
{
    Node* pos = root;
    while(length --)
    {
        if (pos->son[(int)*ptr] == NULL)
            pos->son[(int)*ptr] = newNode();
        pos = pos->son[(int)*ptr];
        ptr ++;
    }
    if (pos->vec.size() == 0 || pos->last != d)
    {
        pos->vec.push_back(d);
        pos->last = d;
    }
    return pos;
}

std::pair<int*, int*> Trie::search(const char* ptr, int length)
{
    Node* pos = root;
    while(length -- && pos)
    {
        pos = pos->son[(int)*ptr];
        ptr ++;
    }
    return pos ? std::make_pair(pos->ds, pos->de) : std::pair<int*, int*>(NULL, NULL);
}

Trie::Node* Trie::newNode()
{
    if (each_pool == NULL || each_tail == EACH_ALLOC) {
        each_pool = new Node[EACH_ALLOC];
        each_tail = 0;
        pools.push_back(each_pool);
    }
    return &each_pool[each_tail++];
}