#include "ED.h"

#include <cstdlib>
#include <cmath>
#include <assert.h>

using namespace std;

bool cmp(const EDJoinResult&A, const EDJoinResult&B)
{
    return A.id2 < B.id2;
}

ED::ED()
{
    dp = new int*[BUFFER_SIZE];
    for(int i = 0; i < BUFFER_SIZE; i ++)
        dp[i] = new int[BUFFER_SIZE];
    checked = new int[FILE_LINE_SIZE];
    checked_flag = 0;
}

ED::~ED()
{
    for(int i = 0; i < BUFFER_SIZE; i ++)
        delete[] dp[i];
    delete[] dp;
    delete[] checked;
}

int ED::join(const STR_FILE&A, const STR_FILE&B, int threshold, std::vector<EDJoinResult> &result)
{
    result.clear();
    
    for(int i = 0; i < FILE_EACH_LINE; i ++)
        for(int j = 0; j < threshold+1; j ++)
        {
            partitions[i][j] = new Hash();
        }
    for(int i = 0; i < FILE_EACH_LINE; i ++)
    {
        pos[i][0] = 0;
        len[i][0] = i/(threshold+1);
        for(int j = 1; j < threshold+1; j ++)
        {
            pos[i][j] = pos[i][j-1] + len[i][j-1];
            len[i][j] = i/(threshold+1) + int(j<i%int(threshold+1));
        }
    }
    
    // build B partitions
    for(int i = 0; i < B.line_size; i ++)
    {
        for(int j = 0; j < threshold+1; j ++)
        {
            partitions[B.lines[i].length][j]->insert(i, B.lines[i].ptr + pos[B.lines[i].length][j], len[B.lines[i].length][j]);
        }
    }
    for(int i = 0; i < FILE_EACH_LINE; i ++)
        for(int j = 0; j < threshold+1; j ++)
        {
            partitions[i][j]->adjust();
        }

    // search
    for(int i = 0; i < A.line_size; i ++)
    {
        const STR s = A.lines[i];
        const int L = s.length;
        const int now_rst_size = result.size();
        checked_flag ++;

        for(int l = max(0, L-threshold), le = L+threshold; l <= le; l ++)
        {
            const int delta = abs(L-l);
            for(int j = 0; j < threshold+1; j ++)
            {
                int ps = 0, pe = L-1;
                ps = max(ps, pos[l][j]-threshold);
                pe = min(pe, pos[l][j]+threshold);
                if (L >= l) {
                    ps = max(ps, pos[l][j]-(threshold-delta)/2);
                } else {
                    pe = min(pe, pos[l][j]+(threshold+delta)/2);
                }
                for(int p = ps; p <= pe; p ++)
                {
                    auto rst = partitions[l][j]->search(s.ptr+p, len[l][j]);
                    for(int *x = rst.first; x != rst.second; x ++)
                    {
                        if (checked[*x] != checked_flag)
                        {
                            int tmp = inDistance(s, B.lines[*x], threshold);
                            if (tmp >= 0)
                            {
                                result.push_back((EDJoinResult){i, *x, tmp});
                            }
                        }
                        checked[*x] = checked_flag;
                    }
                }
            }
        }

        sort(result.begin()+now_rst_size, result.end(), cmp);
    }

    for(int i = 0; i < FILE_EACH_LINE; i ++)
        for(int j = 0; j < threshold+1; j ++)
        {
            delete partitions[i][j];
        }

    return SUCCESS;
}

int ED::inDistance(const STR& A, const STR& B, int threshold)
{
    int n = A.length;
    int m = B.length;
    if(abs(n-m) > threshold) return -1;
    for(int i=0;i<=n;i++)dp[i][0]=i;
    for(int j=0;j<=m;j++)dp[0][j]=j;

    for(int i=1;i<=n;i++)
    {
        for(int j=max(i-threshold, 1);j<=min(m, i+threshold);j++)
        {
            dp[i][j]=dp[i-1][j-1]+1;
            if(abs(i-1-j)<=threshold)dp[i][j]=min(dp[i][j], dp[i-1][j]+1);
            if(abs(j-1-i)<=threshold)dp[i][j]=min(dp[i][j], dp[i][j-1]+1);
            if(A.ptr[i-1]==B.ptr[j-1])dp[i][j]=min(dp[i][j], dp[i-1][j-1]);
        }
    }

    return dp[n][m] <= threshold ? dp[n][m] : -1;
}