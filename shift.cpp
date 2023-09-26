#include <iostream>

using namespace std;

int MAX_BLOCK_SIZE = 2;

struct block {
    int num = 0;
    uint * data;
};

void shiftDown(block** BArray, int BOffset, int offset, int nShift) {

    // To catch invalid input
    if (nShift <= 0 || offset < 0 || BOffset < 0 || offset > MAX_BLOCK_SIZE) {
        return;
    }

    /*
        Created 2 holding areas to store data between blocks.

        The mainHoldingArea stores the data from previous blocks that is
        to be written to the front of the current block. I used calloc here so that
        the empty spaces created by the shift are filled with 0's.
        
        The tempHoldingArea temporarily stores the data at the end of the current block
        as it is overwritten by the shifting procedure.

        The tempHoldingArea data is then transferred to the mainHoldingArea before moving to the next block.
    */
    uint* mainHoldingArea = (uint*)calloc(nShift, sizeof(uint));
    uint* tempHoldingArea = (uint*)malloc(sizeof(uint)*nShift);

    int remainingSpace = MAX_BLOCK_SIZE - (*(BArray + BOffset))->num;
    int toShift;

    
    // *****TODO Need to account for the case when a new block must be created to account for the shift
    // Im assuming that creation of block is done before shifting, and that shifting will not cause data to go out of bounds

    // While there is no empty space at the end of the block such that I can shift without overflow of records.
    while (remainingSpace < nShift) {
        /*
            The number of records that will be shifted and not pushed out of the current block. 
            If the number of shifts is more than the number of records remaining after the offset, toShift is just set to the MAX_BLOCK_SIZE-offset.
            This is because toShift would become negative otherwise.
        */
        toShift = MAX_BLOCK_SIZE - offset - nShift <= 0 ? MAX_BLOCK_SIZE - offset : MAX_BLOCK_SIZE - offset - nShift;

        // Store the records at the end of the block that will be pushed out by the shifting procedure in tempHoldingArea.
        memcpy(tempHoldingArea, &((*(BArray + BOffset))->data[MAX_BLOCK_SIZE-nShift]), sizeof(uint)*nShift);

        // Do the actual shifting of data within the block itself
        memcpy(&((*(BArray + BOffset))->data[offset + nShift]), &((*(BArray + BOffset))->data[offset]), sizeof(uint)*toShift);

        // Write the mainHoldingArea to the front of the block
        memcpy(&((*(BArray + BOffset))->data[offset]), mainHoldingArea, sizeof(uint)*nShift);

        // Since we are moving to new blocks, having an offset no longer makes sense after the first block.
        offset = 0;
        BOffset++;
        remainingSpace = MAX_BLOCK_SIZE - (*(BArray + BOffset))->num;

        // Write the temp to the mainHoldingArea
        memcpy(mainHoldingArea, tempHoldingArea, sizeof(uint)*nShift);
    }

    // One final shift at the last block.
    memcpy(&((*(BArray + BOffset))->data[offset + nShift]), &((*(BArray + BOffset))->data[offset]), sizeof(uint)*toShift);

    // Write the main holding area to the front of the block
    memcpy(&((*(BArray + BOffset))->data[0]), mainHoldingArea, sizeof(uint)*nShift);

    free(mainHoldingArea);
    free(tempHoldingArea);
}

/*
    Assuming that theres no overflow on the block at the top of the chain where the shift is meant to fill gaps,
    and that offset is the starting location of the gap in the original block.

    since shiftUp will only occur if theres a deletion, therefore nShift starting from the offset+nShift within the block should
    not overwrite any existing data.

    *****TO CONFIRM****
*/
void shiftUp(block** BArray, int BOffset, int BArraySize, int offset, int nShift) {
    // To catch invalid input
    if (nShift <= 0 || offset < 0 || BOffset < 0 || offset > MAX_BLOCK_SIZE || BArraySize - 1 < BOffset) {
        return;
    }

    uint* mainHoldingArea = (uint*)calloc(nShift, sizeof(uint));
    uint* tempHoldingArea = (uint*)malloc(sizeof(uint)*nShift);

    for (int i = BArraySize-1; i > BOffset; i--) {
        
        // Shift the front of the array to the tempHoldingArea before it's overwritten
        memcpy(tempHoldingArea, &((*(BArray + i))->data[0]), sizeof(uint)*nShift);

        // Shift the data within the block
        memcpy(&((*(BArray + i))->data[0]), &((*(BArray + i))->data[nShift]), sizeof(uint)*(MAX_BLOCK_SIZE - nShift));

        // Write the mainHoldingArea to the end of the block
        memcpy(&((*(BArray + i))->data[MAX_BLOCK_SIZE - nShift]), mainHoldingArea, sizeof(uint)*nShift);

        // Write the temp to the mainHoldingArea
        memcpy(mainHoldingArea, tempHoldingArea, sizeof(uint)*nShift);
    }

    // Number of records remaining at the end of the original block after the gap
    int toShift = MAX_BLOCK_SIZE - offset - nShift;
    // Do the shifting in the main block that has gaps to fill
    memcpy(&((*(BArray + BOffset))->data[offset]), &((*(BArray + BOffset))->data[offset - nShift]), sizeof(uint)*toShift);

    // Write the mainHoldingArea to the end of the block
    memcpy(&((*(BArray + BOffset))->data[MAX_BLOCK_SIZE - nShift]), mainHoldingArea, sizeof(uint)*nShift);

    free(mainHoldingArea);
    free(tempHoldingArea);
}

int main() {
    block b1;
    block b2;
    block b3;

    b1.data = (uint*)malloc(sizeof(uint)*MAX_BLOCK_SIZE);
    b1.data[0] = 1;
    b1.data[1] = 3;
    b1.num = 2;

    b2.data = (uint*)malloc(sizeof(uint)*MAX_BLOCK_SIZE);
    b2.data[0] = 4;
    b2.data[1] = 5;
    b2.num = 2;

    b3.data = (uint*)malloc(sizeof(uint)*MAX_BLOCK_SIZE);
    b3.data[0] = 6;
    b3.data[1] = 7;
    // b3.num = 2;

    block* BArray[3] = {&b1, &b2, &b3};


    cout << "BEFORE SHIFT" << endl;
    cout << "B1" << endl;
    for (int i = 0; i < 2; i++) {
        cout << b1.data[i] << endl;
    }

    cout << "B2" << endl;
    for (int i = 0; i < 2; i++) {
        cout << b2.data[i] << endl;
    }

    cout << "B3" << endl;
    for (int i = 0; i < 2; i++) {
        cout << b3.data[i] << endl;
    }

    // shiftDown(BArray, 0, 1, 1);
    shiftUp(BArray, 0, 3, 1, 1);

    cout << "AFTER SHIFT" << endl;
    cout << "B1" << endl;
    for (int i = 0; i < 2; i++) {
        cout << b1.data[i] << endl;
    }

    cout << "B2" << endl;
    for (int i = 0; i < 2; i++) {
        cout << b2.data[i] << endl;
    }

    cout << "B3" << endl;
    for (int i = 0; i < 2; i++) {
        cout << b3.data[i] << endl;
    }

    free(b1.data);
    free(b2.data);
    free(b3.data);
}