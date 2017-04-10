#include "SimSearcher.h"
#include "common.h"

using namespace std;

SimSearcher::SimSearcher()
{
    ed = new ED();
    jaccard = new Jaccard();
}

SimSearcher::~SimSearcher()
{
    delete ed;
    delete jaccard;
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{
    if(ed->init(filename) != SUCCESS) return FAILURE;
    if(jaccard->init(filename, q) != SUCCESS) return FAILURE;
	return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	return jaccard->search(string(query), threshold, result);
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
    STR S;
    S.ptr = const_cast<char*>(query);
    S.length = strlen(query);
	return ed->search(S, (int)threshold, result);
}

