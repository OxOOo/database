#ifndef SEARCH_JACCARD_H
#define SEARCH_JACCARD_H

#include "common.h"
#include "Trie.h"
#include "TrieS.h"

#include <vector>
#include <cstdlib>
#include <string>

/// Jaccard
class Jaccard
{
public:
    Jaccard();
    ~Jaccard();

    int init(const char* filename);
	int search(const Str& S, double threshold, std::vector<std::pair<unsigned, double> > &result);

private:
    /// 读取字符串
    void readEntries(const char* filename);
    /// Jaccard 距离
    double inJaccard(const TrieS& query_trie, const int query_size, const StrList& S, double threshold);

    StrList parseStr(const Str& str);

    std::vector<StrList> lines; // 所有语句

    Trie<int> trie;
    TrieS str_trie;

    int *times; // 计算出现的次数
    int *last_modified, last_cnt;
    CON3<int,int*,int*> *lists;
};

#endif // SEARCH_JACCARD_H
