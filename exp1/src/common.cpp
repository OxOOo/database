#include "common.h"

int exist(int* s, int* t, int value)
{
    int* mid;
    while(s + 1 < t)
    {
        mid = s+(t-s)/2;
        if (*mid > value)
            t = mid;
        else s = mid;
    }
    return *s == value;
}