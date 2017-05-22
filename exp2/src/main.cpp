#include "SimJoiner.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    SimJoiner joiner;

    vector<EDJoinResult> resultED;
    vector<JaccardJoinResult> resultJaccard;

    unsigned edThreshold = 2;
    double jaccardThreshold = 0.85;

    const char* filenameA = "../A.txt";
    const char* filenameB = "../B.txt";

    // joiner.joinJaccard(filenameA, filenameB, jaccardThreshold, resultJaccard);
    joiner.joinED(filenameA, filenameB, edThreshold, resultED);

    for(int i = 0; i < (int)resultED.size(); i ++)
    {
        cout << resultED[i].id1 << " " << resultED[i].id2 << " " << resultED[i].s << endl;
    }

    return 0;
}
