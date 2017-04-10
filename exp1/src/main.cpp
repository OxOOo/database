#include "SimSearcher.h"
#include "common.h"

#include <ctime>
#include <iostream>
#include <cstdlib>
#include <assert.h>

using namespace std;

int main(int argc, char **argv)
{
	SimSearcher searcher;

	vector<pair<unsigned, unsigned> > resultED;
	vector<pair<unsigned, double> > resultJaccard;

	unsigned q = 3, edThreshold = 1;
	double jaccardThreshold = 0.5;

    cout << "using input.txt as database" << endl;
    searcher.createIndex("../input.txt", q);
    cout << "create index time : " << clock()/(double)CLOCKS_PER_SEC << endl;

    cout << "using query.txt as query" << endl;
    char buf[BUFFER_SIZE];
    FILE *query_file = fopen("../query.txt", "r");
    int type = 0;
    while(fscanf(query_file, "%d", &type) != EOF)
    {
        if (!fgets(buf, BUFFER_SIZE, query_file)) break;

        int pos = 0;
        while(isspace(buf[pos]) && pos < (int)strlen(buf)) pos ++;
        int end_pos = strlen(buf);
        while(pos < end_pos && isspace(buf[end_pos-1])) end_pos --;
        buf[end_pos] = '\0';

        clock_t s = clock();
        if (type == 1) {
            cout << "searchED : " << buf+pos << endl;
            searcher.searchED(buf+pos, edThreshold, resultED);
            for(int i = 0; i < (int)resultED.size(); i ++)
                cout << resultED[i].first << " " << resultED[i].second << endl;
        } else if (type == 2) {
            cout << "searchJaccard : " << buf+pos << endl;
            searcher.searchJaccard(buf+pos, jaccardThreshold, resultJaccard);
            for(int i = 0; i < (int)resultJaccard.size(); i ++)
                cout << resultJaccard[i].first << " " << resultJaccard[i].second << endl;
        } else {
            assert(false);
        }
        clock_t e = clock();
        
        cout << "time used: " << double(e-s)/CLOCKS_PER_SEC << endl;
    }
    fclose(query_file);
    cout << "total time : " << clock()/(double)CLOCKS_PER_SEC << endl;

	return 0;
}

