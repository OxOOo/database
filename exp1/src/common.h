#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>

#define SUCCESS 0
#define FAILURE 1
#define BUFFER_SIZE 512

struct Str
{
    char* ptr;
    int length;

    Str(char* ptr = NULL, int length = 0): ptr(ptr), length(length) {}
};

struct StrList
{
    Str* strs;
    int size;
};

template<typename T1, typename T2, typename T3>
struct CON3
{
    T1 data1;
    T2 data2;
    T3 data3;

    CON3() = default;
    CON3(T1 data1, T2 data2, T3 data3): data1(data1), data2(data2), data3(data3) {}
    bool operator <(const CON3& B) const {
        return data1 > B.data1;
    }
};

// return true if exist
int exist(int* s, int* t, int value);

#endif // COMMON_H
