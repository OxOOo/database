#ifndef STR_H
#define STR_H

#include "Trie.h"

// 一个字符串
struct STR
{
    char *ptr;
    int df;
    int length;
};

// 字符串集合
struct STR_LIST
{
    STR *strs;
    int size;
};

// 字符串文件
struct STR_FILE
{
    char *filename;
    char *content;
    STR *lines;
    int line_size;
};

bool df_cmp(const STR&A, const STR&B);

void print(const STR&S, const char* tail = NULL);

#endif // STR_H