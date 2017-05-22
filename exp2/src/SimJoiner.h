#ifndef __EXP2_SIMJOINER_H__
#define __EXP2_SIMJOINER_H__

#include "common.h"
#include "str.h"
#include "ED.h"

#include <vector>

class SimJoiner {
public:
    SimJoiner();
    ~SimJoiner();

    int joinJaccard(const char *filename1, const char *filename2, double threshold, std::vector<JaccardJoinResult> &result);
    int joinED(const char *filename1, const char *filename2, unsigned threshold, std::vector<EDJoinResult> &result);

private:
    std::vector<STR_FILE> files;
    ED ed;

    STR_FILE loadFile(const char *filename);
};

#endif
