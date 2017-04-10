#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <cstring>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

#define EACH_ALLOC 10240

template<typename Td>
struct Node
{
    std::vector<Td> vec;
    Td last;
    Td* ds;
    Td* de;
    Node* son[128];

    Node();
};

template<typename Td>
class Trie
{
public:
    Trie();

    void adjust();
    void insert(const Td& d, const char* ptr, int length = -1);
    std::pair<Td*, Td*> search(const char* ptr, int length = -1);

private:
    Node<Td>* root;
    Node<Td>* each_pool;
    int each_tail;
    std::vector<Node<Td>*> pools;

    Node<Td>* newNode();
};

template<typename Td>
Trie<Td>::Trie()
{
    each_pool = NULL;
    each_tail = 0;
    root = newNode();
}

template<typename Td>
Node<Td>::Node()
{
    memset(son, 0, sizeof(son));
}

template<typename Td>
void Trie<Td>::adjust()
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

template<typename Td>
void Trie<Td>::insert(const Td& d, const char* ptr, int length)
{
    if (length == -1) length = std::strlen(ptr);
    Node<Td>* pos = root;
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
}

template<typename Td>
std::pair<Td*, Td*> Trie<Td>::search(const char* ptr, int length)
{
    if (length == -1) length = std::strlen(ptr);
    Node<Td>* pos = root;
    while(length -- && pos)
    {
        pos = pos->son[(int)*ptr];
        ptr ++;
    }
    return pos ? std::make_pair(pos->ds, pos->de) : std::pair<Td*, Td*>(NULL, NULL);
}

template<typename Td>
Node<Td>* Trie<Td>::newNode()
{
    if (each_pool == NULL || each_tail == EACH_ALLOC) {
        each_pool = new Node<Td>[EACH_ALLOC];
        each_tail = 0;
        pools.push_back(each_pool);
    }
    return &each_pool[each_tail++];
}

#endif // TRIE_H