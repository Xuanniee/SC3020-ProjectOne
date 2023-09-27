#ifndef SHIFT
#define SHIFT

typedef unsigned int uint;

struct block {
    int num = 0;
    uint * data;
};

/*
    @param BArray - pointer to the array of block pointers
    @param BOffset - the index of the target block
    @param BArraySize - the size of the array of blocks
    @param offset - the index of the target record within the block
    @param nShift - the number of records to shift by
*/
void shiftUp(block** BArray, int BOffset, int BArraySize, int offset, int nShift);

/*
    @param BArray - pointer to the array of block pointers
    @param BOffset - the index of the target block
    @param offset - the index of the target record within the block
    @param nShift - the number of records to shift by
*/
void shiftDown(block** BArray, int BOffset, int offset, int nShift);

#endif
