#include "SimJoiner.h"

#include <cstring>
#include <cstdio>
#include <cctype>
#include <iostream>
#include <assert.h>

using namespace std;

SimJoiner::SimJoiner() {
}

SimJoiner::~SimJoiner() {
    for(int i = 0; i < (int)files.size(); i ++)
    {
        delete[] files[i].filename;
        delete[] files[i].content;
        delete[] files[i].lines;
    }
    files.clear();
}

int SimJoiner::joinJaccard(const char *filename1, const char *filename2, double threshold, vector<JaccardJoinResult> &result) {
    STR_FILE fileA = loadFile(filename1);
    STR_FILE fileB = loadFile(filename2);

    jac.join(fileA, fileB, threshold, result);

    return SUCCESS;
}

int SimJoiner::joinED(const char *filename1, const char *filename2, unsigned threshold, vector<EDJoinResult> &result) {
    STR_FILE fileA = loadFile(filename1);
    STR_FILE fileB = loadFile(filename2);

    ed.join(fileA, fileB, (int)threshold, result);

    return SUCCESS;
}

STR_FILE SimJoiner::loadFile(const char *filename)
{
    for(int i = 0; i < (int)files.size(); i ++)
        if (strcmp(filename, files[i].filename) == 0)
            return files[i];
    
    STR_FILE file;

    int filename_len = strlen(filename) + 1;
    file.filename = new char[filename_len];
    memcpy(file.filename, filename, sizeof(char)*filename_len);

    file.content = new char[FILE_LINE_SIZE*FILE_EACH_LINE];
    file.lines = new STR[FILE_LINE_SIZE];
    file.line_size = 0;

    FILE *fd = fopen(filename, "rb");
    int end = fread(file.content, sizeof(char), FILE_LINE_SIZE*FILE_EACH_LINE, fd);
    fclose(fd);

    for(int pos = 0; pos < end;)
    {
        int offset = pos;
        while(offset < end && file.content[offset] != '\n') offset ++;

        file.lines[file.line_size].ptr = file.content + pos;
        file.lines[file.line_size].length = offset - pos;

        pos = offset+1;

        file.lines[file.line_size].ptr[file.lines[file.line_size].length] = '\0';
        file.line_size ++;
    }

    files.push_back(file);
    return file;
}