#ifndef SIM_SEARCHER_H
#define SIM_SEARCHER_H

#include <vector>
#include <utility>
#include <string>

#include "common.h"
#include "ED.h"
#include "Jaccard.h"

class SimSearcher
{
public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);

private:
    ED *ed;
    Jaccard *jaccard;
};

#endif // SIM_SEARCHER_H
