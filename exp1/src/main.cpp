#include "SimSearcher.h"

#include <ctime>
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv)
{
	SimSearcher searcher;

	vector<pair<unsigned, unsigned> > resultED;
	vector<pair<unsigned, double> > resultJaccard;

	unsigned q = 3, edThreshold = 1;
	// double jaccardThreshold = 0.85;

    cout << "using input.txt as database" << endl;
    searcher.createIndex("../input.txt", q);
    cout << "create index time : " << clock()/(double)CLOCKS_PER_SEC << endl;

    cout << "using query.txt as query" << endl;
    char buf[1024];
    FILE *query_file = fopen("../query.txt", "r");
    while(fscanf(query_file, "%s", buf) != EOF)
    {
        clock_t s = clock();
        searcher.searchED(buf, edThreshold, resultED);
        clock_t e = clock();
        cout << "time used: " << double(e-s)/CLOCKS_PER_SEC << endl;
        cout << "search : " << buf << endl;
        for(int i = 0; i < (int)resultED.size(); i ++)
            cout << resultED[i].first << " " << resultED[i].second << endl;
    }
    fclose(query_file);
    cout << "total time : " << clock()/(double)CLOCKS_PER_SEC << endl;

    /*
	searcher.searchJaccard("query", jaccardThreshold, resultJaccard);
	searcher.searchED("query", edThreshold, resultED);

    cout << "ED resuilt:" << endl;
    for(int i=0;i<(int)resultED.size();i++)
        cout << resultED[i].first << " " << resultED[i].second << endl;
    */

	return 0;
}

