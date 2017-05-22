#ifndef COMMON_H
#define COMMON_H

template <typename IDType, typename SimType>
struct JoinResult {
    IDType id1;
    IDType id2;
    SimType s;
};

typedef JoinResult<unsigned, double> JaccardJoinResult;
typedef JoinResult<unsigned, unsigned> EDJoinResult;

const int SUCCESS = 0;
const int FAILURE = 1;

const int FILE_LINE_SIZE = 200000 + 10;
const int FILE_EACH_LINE = 256 + 10;
const int BUFFER_SIZE = 512;
const int MAX_TAU = 3;

#endif // COMMON_H