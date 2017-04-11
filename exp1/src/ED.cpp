#include "ED.h"

#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstdlib>

using namespace std;

ED::ED()
{
    dp = new int*[BUFFER_SIZE];
    for(int i = 0; i < BUFFER_SIZE; i ++)
        dp[i] = new int[BUFFER_SIZE];
    times = last_modified = NULL;

    lists = new CON3<int,int*,int*>[BUFFER_SIZE];
}

ED::~ED()
{
    for(int i = 0; i < BUFFER_SIZE; i ++)
        delete[] dp[i];
    delete[] dp;

    for(int i = 0; i < (int)entries.size(); i ++)
        delete[] entries[i].ptr;
    entries.clear();

    if (times) delete[] times;
    if (last_modified) delete[] last_modified;
    delete[] lists;
}

void ED::readEntries(const char* filename)
{
    FILE *file = fopen(filename, "r");
    char *line = new char[BUFFER_SIZE];

    while(fgets(line, BUFFER_SIZE, file))
    {
        int len = strlen(line);
        while(len && isspace(line[len-1]))len--;
        line[len]='\0';
        assert(len != 0);

        Str str;
        str.ptr = new char[BUFFER_SIZE];
        str.length = len;
        memcpy(str.ptr, line, sizeof(char)*BUFFER_SIZE);
        entries.push_back(str);
    }

    fclose(file);
    delete[] line;
}

int ED::init(const char* filename)
{
    readEntries(filename);

    entries_ids.resize(entries.size());
    for(int i = 0; i < (int)entries_ids.size(); i ++)
        entries_ids[i] = i;
    sort(entries_ids.begin(), entries_ids.end(), [&](const int&a, const int&b) {
        return entries[a].length < entries[b].length;
    }); // 按照字符串长度排序

    // build index
    for(int i = 0; i < (int)Qs.size(); i ++)
    {
        const int Q = Qs[i];
        
        for(int id = 0; id < (int)entries.size(); id ++)
        {
            for(int k = 0; k + Q <= entries[id].length; k ++)
                trie.insert(id, entries[id].ptr+k, Q);
        }
    }
    trie.adjust();

    times = new int[entries.size()];
    last_modified = new int[entries.size()];
    last_cnt = 0;
    memset(times, 0, sizeof(int)*entries.size());
    memset(last_modified, 0, sizeof(int)*entries.size());

    return SUCCESS;
}

int ED::search(const Str& S, int threshold, vector<pair<unsigned, unsigned> > &result)
{
    result.clear();
    result.reserve(BUFFER_SIZE*10);

    for(int i = 0; i < (int)Qs.size(); i ++)
    {
        const int Q = Qs[i];
        const int T = S.length - threshold*Q - Q + 1;
        if (T > 0) return trieSearch(S, threshold, result, Q, &trie);
    }

    int l = 0, r = entries_ids.size(), mid;
    while(l < r)
    {
        mid = (l+r)/2;
        if (entries[entries_ids[mid]].length >= S.length - threshold)
            r = mid;
        else l = mid + 1;
    }
    int s = l;

    l = 0, r = entries_ids.size();
    while(l < r)
    {
        mid = (l+r)/2;
        if (entries[entries_ids[mid]].length > S.length + threshold)
            r = mid;
        else l = mid + 1;
    }
    int t = l;

    for(int i = s; i < t; i ++)
    {
        int temp = inDistance(S, entries[entries_ids[i]], threshold);
        if (temp >= 0) result.push_back(make_pair(entries_ids[i], temp));
    }
    sort(result.begin(), result.end());

    return SUCCESS;
}

int ED::trieSearch(const Str& S, int threshold, std::vector<std::pair<unsigned, unsigned> > &result, const int Q, Trie<int>* trie)
{
    const int T = S.length - threshold*Q - Q + 1;
    last_cnt ++;

    int lists_cnt = 0;
    for(int i = 0; i + Q <= S.length; i ++)
    {
        auto x = trie->search(S.ptr+i, Q);
        if (!x.first || !x.second) continue;
        lists[lists_cnt++] = CON3<int,int*,int*>(x.second-x.first, x.first, x.second);
    }
    sort(lists, lists+lists_cnt);

    for(int k = T-1; k < lists_cnt; k ++)
    {
        auto s = lists[k].data2, t = lists[k].data3;

        for(auto p = s; p != t; p ++)
        {
            times[*p] = times[*p]*int(last_modified[*p] == last_cnt) + 1;
            last_modified[*p] = last_cnt;
        }
    }

    for(int k = T-1; k < lists_cnt; k ++)
    {
        auto s = lists[k].data2, t = lists[k].data3;

        for(auto p = s; p != t; p ++)
        {
            if (last_modified[*p] != last_cnt) continue;
            last_modified[*p] = last_cnt - 1;
            for(int i = 0; i < T-1 && times[*p] < T && times[*p]+(T-1-i)>=T; i ++)
                times[*p] += exist(lists[i].data2, lists[i].data3, *p);

            if (times[*p] >= T)
            {
                int temp = inDistance(S, entries[*p], threshold);
                if (temp >= 0) result.push_back(make_pair(*p, temp));
            }
        }
    }

    sort(result.begin(), result.end());

    return SUCCESS;
}

int ED::inDistance(const Str& A, const Str& B, int threshold)
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