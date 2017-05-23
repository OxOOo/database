#include "Jaccard.h"
#include "Hash.h"

#include <assert.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

bool cmp(const JaccardJoinResult&A, const JaccardJoinResult&B)
{
    return A.id2 < B.id2;
}

Jaccard::Jaccard()
{
    checked = new int[FILE_LINE_SIZE];
    checked_flag = 0;
}

Jaccard::~Jaccard()
{
    delete[] checked;
}

int Jaccard::join(const STR_FILE&A, const STR_FILE&B, double threshold, std::vector<JaccardJoinResult> &result)
{
    result.clear();

    vector<STR_LIST> Alines, Blines;
    for(int i = 0; i < A.line_size; i ++)
    {
        Alines.push_back(parseStr(A.lines[i]));
    }
    for(int i = 0; i < B.line_size; i ++)
    {
        Blines.push_back(parseStr(B.lines[i]));
    }

    // calc df
    Hash df(9997201);
    for(int i = 0; i < A.line_size; i ++)
        for(int j = 0; j < Alines[i].size; j ++)
            df.insert(i, Alines[i].strs[j].ptr, Alines[i].strs[j].length);
    for(int i = 0; i < B.line_size; i ++)
        for(int j = 0; j < Blines[i].size; j ++)
            df.insert(-i, Blines[i].strs[j].ptr, Blines[i].strs[j].length);
    df.adjust();
    for(int i = 0; i < A.line_size; i ++)
    {
        for(int j = 0; j < Alines[i].size; j ++)
        {
            auto s = df.search(Alines[i].strs[j].ptr, Alines[i].strs[j].length);
            Alines[i].strs[j].df = s.second - s.first;
        }
        sort(Alines[i].strs, Alines[i].strs+Alines[i].size, df_cmp);
    }
    for(int i = 0; i < B.line_size; i ++)
    {
        for(int j = 0; j < Blines[i].size; j ++)
        {
            auto s = df.search(Blines[i].strs[j].ptr, Blines[i].strs[j].length);
            Blines[i].strs[j].df = s.second - s.first;
        }
        sort(Blines[i].strs, Blines[i].strs+Blines[i].size, df_cmp);
    }

    // build index
    Hash index(9997201);
    for(int i = 0; i < B.line_size; i ++)
    {
        int size = Blines[i].size-ceil(threshold*Blines[i].size-1);
        for(int j = 0; j < size; j ++)
            index.insert(i, Blines[i].strs[j].ptr, Blines[i].strs[j].length);
    }
    index.adjust();

    // search
    TrieS th;
    for(int i = 0; i < A.line_size; i ++)
    {
        const STR_LIST s = Alines[i];
        checked_flag ++;
        const int now_rst_size = result.size();
        th.init();
        for(int j = 0; j < s.size; j ++)
            th.insert(s.strs[j].ptr, s.strs[j].length);
        
        for(int j = 0; j < s.size; j ++)
        {
            auto info = index.search(s.strs[j].ptr, s.strs[j].length);
            for(int *x = info.first; x != info.second; x ++)
            {
                if (checked[*x] != checked_flag)
                {
                    double tmp = inJaccard(th, s.size, Blines[*x], threshold);
                    if (tmp >= 0)
                    {
                        result.push_back((JaccardJoinResult){i, *x, tmp});
                    }
                }
                checked[*x] = checked_flag;
            }
        }

        sort(result.begin()+now_rst_size, result.end(), cmp);
    }

    for(int i = 0; i < A.line_size; i ++)
        delete[] Alines[i].strs;
    for(int i = 0; i < B.line_size; i ++)
        delete[] Blines[i].strs;

    return SUCCESS;
}

STR_LIST Jaccard::parseStr(const STR& str)
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

    STR_LIST list;
    list.strs = new STR[grams.size()];
    list.size = grams.size();
    for(int i = 0; i < (int)grams.size(); i ++)
    {
        STR s;
        s.length = grams[i].second - grams[i].first;
        s.ptr = grams[i].first;
        list.strs[i] = s;
    }

    static TrieS T;
    T.init();
    for(int i = 0; i < list.size; )
    {
        if (!T.insert(list.strs[i].ptr, list.strs[i].length))
        {
            list.size --;
            swap(list.strs[i], list.strs[list.size]);
        } else i ++;
    }

    return list;
}

double Jaccard::inJaccard(const TrieS& query_trie, const int query_size, const STR_LIST& S, double threshold)
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