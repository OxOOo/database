#ifndef STR_H
#define STR_H

#include "Trie.h"

// 一个字符串
struct STR
{
    char *ptr;
    int length;
};

// 字符串集合
struct STR_SET
{
    STR *sets;
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

// gram
struct GRAM
{
    char* ptr;
    int df;
    Trie::Node* node;
};

bool gram_cmp(const GRAM&A, const GRAM&B);

#endif // STR_H