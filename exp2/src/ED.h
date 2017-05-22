#ifndef ED_H
#define ED_H

#include "common.h"
#include "str.h"
#include "Hash.h"

#include <vector>

/// 编辑距离
class ED
{
public:
    ED();
    ~ED();

    int join(const STR_FILE&A, const STR_FILE&B, int threshold, std::vector<EDJoinResult> &result);
private:
    /// 字符串A和B之间的编辑距离是否在threshold之间,否则返回-1
    int inDistance(const STR& A, const STR& B, int threshold);

    int **dp; // 用于计算两个字符串的ED
    Hash* partitions[FILE_EACH_LINE][MAX_TAU+2];
    int pos[FILE_EACH_LINE][MAX_TAU+2], len[FILE_EACH_LINE][MAX_TAU+2];
    int *checked, checked_flag;
};

#endif // ED_H