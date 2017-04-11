#include "Jaccard.h"

#include <assert.h>
#include <iostream>

using namespace std;

Jaccard::Jaccard()
{
    times = last_modified = NULL;
    lists = new CON3<int,int*,int*>[BUFFER_SIZE];
}

Jaccard::~Jaccard()
{
    if (times) delete[] times;
    if (last_modified) delete[] last_modified;
    delete[] lists;

    for(int i = 0; i < (int)lines.size(); i ++)
        delete[] lines[i].mem;
    lines.clear();
}

int Jaccard::init(const char* filename)
{
    readEntries(filename);

    // build index
    for(int id = 0; id < (int)lines.size(); id ++)
    {
        for(int k = 0; k < lines[id].size; k ++)
        {
            trie.insert(id, lines[id].strs[k].ptr, lines[id].strs[k].length);
        }
    }
    trie.adjust();

    for(int id = 0; id < (int)lines.size(); id ++)
    {
        for(int i = 0; i < lines[id].size; i ++)
            for(int j = 0; j < i; j ++)
                if (lines[id].strs[i].length == lines[id].strs[j].length && strncmp(lines[id].strs[i].ptr, lines[id].strs[j].ptr, lines[id].strs[i].length) == 0)
                {
                    lines[id].strs[i].length = -1;
                    break;
                }
    }

    times = new int[lines.size()];
    last_modified = new int[lines.size()];
    last_cnt = 0;
    memset(times, 0, sizeof(int)*lines.size());
    memset(last_modified, 0, sizeof(int)*lines.size());

    return SUCCESS;
}

int Jaccard::search(const Str& S, double threshold, vector<pair<unsigned, double> > &result)
{
    result.clear();
    result.reserve(BUFFER_SIZE*10);

    StrList str_list = parseStr(S);
    str_trie.init();
    int str_size = 0;
    for(int i = 0; i < str_list.size; i ++)
    {
        int ret = str_trie.insert(str_list.strs[i].ptr, str_list.strs[i].length);
        str_size += ret;
        if (!ret) str_list.strs[i].length = -1;
    }

    const int T = max(1, (int)ceil(str_size*threshold));
    last_cnt ++;

    int lists_cnt = 0;
    for(int i = 0; i < str_list.size; i ++)
        if (str_list.strs[i].length >= 0)
        {
            auto x = trie.search(str_list.strs[i].ptr, str_list.strs[i].length);
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
                auto temp = inJaccard(str_trie, str_size, lines[*p], threshold);
                if (temp >= 0) result.push_back(make_pair(*p, temp));
            }
        }
    }

    sort(result.begin(), result.end());

    return SUCCESS;
}

void Jaccard::readEntries(const char* filename)
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
        lines.push_back(parseStr(str));
    }

    fclose(file);
    delete[] line;
}

StrList Jaccard::parseStr(const Str& str)
{
    vector<pair<char*,char*>> grams;
    for(int s = 0, t; s < str.length;)
    {
        while(isspace(str.ptr[s]) && s < str.length) s ++;
        if (s >= str.length) break;
        t = s;
        do t ++; while(t < str.length && !isspace(str.ptr[t]));
        grams.push_back(make_pair(str.ptr+s, str.ptr+t));
        s = t;
    }

    StrList list;
    list.strs = new Str[grams.size()];
    list.mem = str.ptr;
    list.size = grams.size();
    for(int i = 0; i < (int)grams.size(); i ++)
    {
        Str s;
        s.length = grams[i].second - grams[i].first;
        s.ptr = grams[i].first;
        list.strs[i] = s;
    }

    return list;
}

double Jaccard::inJaccard(const TrieS& query_trie, const int query_size, const StrList& S, double threshold)
{
    int A = 0, B = query_size;
    for(int i = 0; i < S.size; i ++)
        if (S.strs[i].length >= 0)
        {
            if (!query_trie.exist(S.strs[i].ptr, S.strs[i].length))
                B ++;
            else A ++;
        }
    
    double t = double(A)/double(B);
    return t < threshold ? -1.0 : t;
}