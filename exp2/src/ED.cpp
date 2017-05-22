#include "ED.h"

#include <cstdlib>

using namespace std;

bool cmp(const EDJoinResult&A, const EDJoinResult&B)
{
    return A.id1 < B.id1 || (A.id1 == B.id1 && A.id2 < B.id2);
}

ED::ED()
{
    dp = new int*[BUFFER_SIZE];
    for(int i = 0; i < BUFFER_SIZE; i ++)
        dp[i] = new int[BUFFER_SIZE];
    grams = new GRAM[FILE_LINE_SIZE*FILE_EACH_LINE*2];
    checked = new int[FILE_LINE_SIZE];
    checked_flag = 0;
}

ED::~ED()
{
    for(int i = 0; i < BUFFER_SIZE; i ++)
        delete[] dp[i];
    delete[] dp;
    for(int i = 0; i < (int)tries.size(); i ++)
        delete tries[i];
    delete[] grams;
    delete[] checked;
}

int ED::join(const STR_FILE&A, const STR_FILE&B, unsigned threshold, std::vector<EDJoinResult> &result)
{
    result.clear();
    df_trie = new Trie();
    tries.push_back(df_trie);
    index_trie = new Trie();
    tries.push_back(index_trie);

    int gram_offset;

    // calc df
    gram_offset = 0;
    for(int i = 0; i < B.line_size; i ++)
    {
        for(int j = 0; j + Q <= B.lines[i].length; j ++)
        {
            grams[gram_offset].ptr = B.lines[i].ptr + j;
            grams[gram_offset].node = df_trie->insert(i, B.lines[i].ptr + j, Q);
            gram_offset ++;
        }
    }
    for(int i = 0; i < A.line_size; i ++)
    {
        for(int j = 0; j + Q <= A.lines[i].length; j ++)
        {
            grams[gram_offset].ptr = A.lines[i].ptr + j;
            grams[gram_offset].node = df_trie->insert(-i, A.lines[i].ptr + j, Q);
            gram_offset ++;
        }
    }
    df_trie->adjust();
    for(int i = 0; i < gram_offset; i ++)
    {
        grams[i].df = grams[i].node->de - grams[i].node->ds;
    }

    // build index
    gram_offset = 0;
    for(int i = 0; i < B.line_size; i ++)
    {
        int size = B.lines[i].length - Q + 1;

        sort(grams+gram_offset, grams+gram_offset+size, gram_cmp);

        for(int j = 0; j < size && j <= Q*threshold+1; j ++)
        {
            index_trie->insert(i, grams[gram_offset+j].ptr, Q);
        }
        gram_offset += size;
    }
    index_trie->adjust();

    // search
    for(int i = 0; i < A.line_size; i ++)
    {
        int size = A.lines[i].length - Q + 1;
        checked_flag ++;
        sort(grams+gram_offset, grams+gram_offset+size, gram_cmp);

        for(int j = 0; j < size && j <= Q*threshold+1; j ++)
        {
            auto info = index_trie->search(grams[gram_offset+j].ptr, Q);
            for(int *x = info.first; x != info.second; x ++)
                if (checked[*x] != checked_flag)
                {
                    checked[*x] = checked_flag;
                    int dis = inDistance(A.lines[i], B.lines[*x], threshold);
                    if (dis != -1)
                    {
                        result.push_back((EDJoinResult){(unsigned)i, (unsigned)*x, (unsigned)dis});
                    }
                }
        }

        gram_offset += size;
    }
    sort(result.begin(), result.end(), cmp);

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