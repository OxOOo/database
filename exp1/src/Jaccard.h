#ifndef SEARCH_JACCARD_H
#define SEARCH_JACCARD_H

#include "common.h"

#include <vector>
#include <cstdlib>
#include <string>

/// Jaccard
class Jaccard
{
public:
    Jaccard();
    ~Jaccard();

    int init(const char* filename, unsigned q);
	int search(const std::string query, double threshold, std::vector<std::pair<unsigned, double> > &result);
};

#endif // SEARCH_JACCARD_H
