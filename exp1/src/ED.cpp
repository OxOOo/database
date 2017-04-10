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
}

ED::~ED()
{
    for(int i = 0; i < BUFFER_SIZE; i ++)
        delete[] dp[i];
    delete[] dp;

    for(int i = 0; i < (int)entries.size(); i ++)
        delete[] entries[i].ptr;
    entries.clear();
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

        STR str;
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
    for(int id = 0; id < (int)entries.size(); id ++)
    {
        for(int k = 0; k + Q <= entries[id].length; k ++)
            trie.insert(entries[id].ptr+k, Q)->insert(id);
    }

    return SUCCESS;
}

int ED::search(const STR& S, int threshold, vector<pair<unsigned, unsigned> > &result)
{
    result.clear();

    const int T = S.length - threshold*Q - Q + 1;

    if (T <= 0)
    {
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

    } else {
        vector<set<int>*> list;
        for(int i = 0; i + Q <= S.length; i ++)
        {
            auto x = trie.search(S.ptr+i, Q);
            if (x) list.push_back(x);
        }

        priority_queue<CON3<int, set<int>::iterator, set<int>::iterator>> que;
        int count = 0, value = -1;
        for(auto x: list) que.push(CON3<int, set<int>::iterator, set<int>::iterator>(*x->begin(), x->begin(), x->end()));
        while(!que.empty())
        {
            auto p = que.top(); que.pop();
            int v = p.data1;
            auto it = p.data2;
            auto end = p.data3;

            if (v == value) count ++;
            else {
                if (value >= v) exit(1);
                count = 1; value = v;
            }
            // cout << "debug : " << value << " " << count << " " << T << endl;
            if (count == T)
            {
                int temp = inDistance(S, entries[value], threshold);
                if (temp >= 0) result.push_back(make_pair(value, temp));
            }
            
            it ++;
            if (it != end)
            {
                que.push(CON3<int, set<int>::iterator, set<int>::iterator>(*it, it, end));
            }
        }
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
        bool flag = false;
        for(int j=max(i-threshold, 1);j<=min(m, i+threshold);j++)
        {
            dp[i][j]=dp[i-1][j-1]+1;
            if(abs(i-1-j)<=threshold)dp[i][j]=min(dp[i][j], dp[i-1][j]+1);
            if(abs(j-1-i)<=threshold)dp[i][j]=min(dp[i][j], dp[i][j-1]+1);
            if(A.ptr[i-1]==B.ptr[j-1])dp[i][j]=min(dp[i][j], dp[i-1][j-1]);
            flag |= dp[i][j]<=threshold;
        }
        if (!flag) return -1;
    }

    return dp[n][m] <= threshold ? dp[n][m] : -1;
}
