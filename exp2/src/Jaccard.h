#ifndef SEARCH_JACCARD_H
#define SEARCH_JACCARD_H

#include "common.h"
#include "str.h"
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

    int join(const STR_FILE&A, const STR_FILE&B, double threshold, std::vector<JaccardJoinResult> &result);

private:
    /// Jaccard 距离
    double inJaccard(const TrieS& query_trie, const int query_size, const STR_LIST& S, double threshold);

    STR_LIST parseStr(const STR& str);

    int *checked, checked_flag;
};

#endif // SEARCH_JACCARD_H
