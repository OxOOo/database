#include "str.h"

#include <cstdlib>
#include <cstdio>

using namespace std;

bool df_cmp(const STR&A, const STR&B)
{
    return A.df < B.df;
}

void print(const STR&S, const char* tail)
{
    for(int i = 0; i < S.length; i ++)
        putchar(S.ptr[i]);
    if (tail) printf("%s", tail);
}