#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <cstring>
#include <iostream>

#define EACH_ALLOC 1024

template<typename Td>
struct Node
{
    Td data;
    Node* son[128];

    Node();
};

template<typename Td>
class Trie
{
public:
    Trie();

    Td* insert(const char* ptr, int length = -1);
    Td* search(const char* ptr, int length = -1);

private:
    Node<Td>* root;
    Node<Td>* each_pool;
    int each_tail;

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
Td* Trie<Td>::insert(const char* ptr, int length)
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
    return &pos->data;
}

template<typename Td>
Td* Trie<Td>::search(const char* ptr, int length)
{
    if (length == -1) length = std::strlen(ptr);
    Node<Td>* pos = root;
    while(length -- && pos)
    {
        pos = pos->son[(int)*ptr];
        ptr ++;
    }
    return pos ? &pos->data : NULL;
}

template<typename Td>
Node<Td>* Trie<Td>::newNode()
{
    if (each_pool == NULL || each_tail == EACH_ALLOC) {
        each_pool = new Node<Td>[EACH_ALLOC];
        each_tail = 0;
    }
    return &each_pool[each_tail++];
}

#endif // TRIE_H