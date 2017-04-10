#ifndef SEARCH_ED_H
#define SEARCH_ED_H

#include "common.h"
#include "Trie.h"

#include <vector>
#include <cstdlib>
#include <cstring>
#include <map>
#include <set>
#include <cstdio>
#include <assert.h>
#include <algorithm>
#include <set>
#include <vector>

/// 编辑距离
class ED
{
public:
    ED();
    ~ED();
    const std::vector<int> Qs = {8, 6, 4};

    int init(const char* filename);
	int search(const STR& S, int threshold, std::vector<std::pair<unsigned, unsigned> > &result);
private:
    ///
    int trieSearch(const STR& S, int threshold, std::vector<std::pair<unsigned, unsigned> > &result, const int Q, Trie<int>* trie);
    /// 字符串A和B之间的编辑距离是否在threshold之间,否则返回-1
    int inDistance(const STR& A, const STR& B, int threshold);
    /// 读取字符串
    void readEntries(const char* filename);

    std::vector<STR> entries; // 所有单词
    std::vector<int> entries_ids; // 按照单词的长度排序
    std::vector<Trie<int>*> tries;

    int **dp; // 用于计算两个字符串的ED
};

#endif // SEARCH_ED_H
