#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <cstring>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

#define EACH_ALLOC 1024

class Trie
{
public:
    struct Node
    {
        std::vector<int> vec;
        int last;
        int* ds;
        int* de;
        Node* son[128];

        Node();
    };

    Trie();
    ~Trie();

    void adjust();
    Node* insert(const int& d, const char* ptr, int length);
    std::pair<int*, int*> search(const char* ptr, int length);

private:
    Node* root;
    Node* each_pool;
    int each_tail;
    std::vector<Node*> pools;

    Node* newNode();
};

#endif // TRIE_H