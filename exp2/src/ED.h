#ifndef ED_H
#define ED_H

#include "common.h"
#include "str.h"
#include "Trie.h"

#include <vector>

/// 编辑距离
class ED
{
public:
    ED();
    ~ED();
    const int Q = 4;

    int join(const STR_FILE&A, const STR_FILE&B, unsigned threshold, std::vector<EDJoinResult> &result);
private:
    /// 字符串A和B之间的编辑距离是否在threshold之间,否则返回-1
    int inDistance(const STR& A, const STR& B, int threshold);

    int **dp; // 用于计算两个字符串的ED
    int *checked, checked_flag;
    std::vector<Trie*> tries;
    Trie *df_trie;
    Trie *index_trie;
    GRAM *grams;
};

#endif // ED_H