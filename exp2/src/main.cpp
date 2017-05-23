#include "SimJoiner.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    SimJoiner joiner;

    vector<EDJoinResult> resultED;
    vector<JaccardJoinResult> resultJaccard;

    unsigned edThreshold = 2;
    double jaccardThreshold = 0.75;

    const char* filenameA = "../A.txt";
    const char* filenameB = "../B.txt";

    joiner.joinJaccard(filenameA, filenameB, jaccardThreshold, resultJaccard);
    joiner.joinED(filenameA, filenameB, edThreshold, resultED);

    for(int i = 0; i < (int)resultJaccard.size(); i ++)
    {
        cout << resultJaccard[i].id1 << " " << resultJaccard[i].id2 << " " << resultJaccard[i].s << endl;
    }

    return 0;
}
