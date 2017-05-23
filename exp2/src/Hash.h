#ifndef HASH_H
#define HASH_H

#include <vector>

#define P 157

typedef unsigned long long ULL;

class Hash
{
public:
    Hash(int MOD = 100007);
    ~Hash();

    void adjust();
    void insert(const int& d, const char* ptr, int length);
    std::pair<int*, int*> search(const char* ptr, int length) const;

private:
    const int MOD;
    std::vector<int> *buckets;
    int **ds, **de;

    void alloc();
    ULL calcHash(const char* ptr, int length) const;
};

#endif // HASH_H