#include <iostream>

#define WORD_SIZE 4

using namespace std;

#ifndef PIPELINE
#define PIPELINE
class Pipeline
{
    public:
    void fetchNextInstruction();
};
#endif