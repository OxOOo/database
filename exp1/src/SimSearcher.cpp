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

int SimSearcher::createIndex(const char *filename, unsigned)
{
    if(ed->init(filename) != SUCCESS) return FAILURE;
    if(jaccard->init(filename) != SUCCESS) return FAILURE;
	return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
    Str S;
    S.ptr = const_cast<char*>(query);
    S.length = strlen(query);
	return jaccard->search(S, threshold, result);
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
    Str S;
    S.ptr = const_cast<char*>(query);
    S.length = strlen(query);
	return ed->search(S, (int)threshold, result);
}

