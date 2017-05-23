#include "Hash.h"

#include <cstdlib>

using namespace std;

Hash::Hash(int MOD): MOD(MOD)
{
    buckets = NULL;
    ds = de = NULL;
}

Hash::~Hash()
{
    if (buckets) delete[] buckets;
    if (ds) delete[] ds;
    if (de) delete[] de;
}

void Hash::adjust()
{
    if (buckets == NULL) return;
    for(int i = 0; i < MOD; i ++)
    {
        if (buckets[i].size() >= 0)
        {
            ds[i] = &buckets[i][0];
            de[i] = ds[i] + buckets[i].size();
        } else {
            ds[i] = de[i] = NULL;
        }
    }
}

void Hash::insert(const int& d, const char* ptr, int length)
{
    alloc();

    ULL h = calcHash(ptr, length);
    buckets[h%MOD].push_back(d);
}

std::pair<int*, int*> Hash::search(const char* ptr, int length) const
{
    if (buckets == NULL) return std::pair<int*, int*>(NULL, NULL);
    ULL h = calcHash(ptr, length)%MOD;
    return make_pair(ds[h], de[h]);
}

void Hash::alloc()
{
    if (buckets == NULL)
    {
        buckets = new vector<int>[MOD];
        ds = new int*[MOD];
        de = new int*[MOD];
    }
}

ULL Hash::calcHash(const char* ptr, int length) const
{
    ULL ans = 0;
    for(int i = 0; i < length; i ++)
        ans = ans*P + ptr[i];
    return ans;
}