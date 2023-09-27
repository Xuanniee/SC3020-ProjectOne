// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <blockManager.h>

/**
 * Helper Functions from the rest to prevent syntax errors
 */
float byteToFloat(unsigned char *recordKeyBitArray) {}


DataBlock* BlockManager :: createDataBlock() {
    // Fix the Size of the Data Block to 400B
    size_t dataBlockSize = 400;

    // Malloc 400 Bytes of Space for the Data Block
    DataBlock *newDataBlock = (DataBlock*) malloc(dataBlockSize);

    // Verify Memory Allocation was successful
    if (newDataBlock == NULL) {
        std::cout << "Memory Allocation Failed." << std::endl;
        exit(0);
    }
    else {
        std::cout << "Memory Allocation Successful. One Data Block of 400B has been initialised." << std::endl;
    }

    // Add to the Block Manager's Array of Blocks, not Records since it should be handled by insertRecord
    this->listBlocks[this->numDataBlocks] = newDataBlock;
    this->numDataBlocks++;

    return newDataBlock;
}


int BlockManager :: deleteDataBlock(DataBlock *blockToDelete) {
    // Retrieve the Data Block to be deleted
    int numDataBlocks = this->numDataBlocks;
    int deletedIndexBlock = -1;

    // Iterate until we find the position of the block to be deleted
    for (int i{0}; i < numDataBlocks; i++) {
        DataBlock *currBlock = this->listBlocks[i];
        if (currBlock == blockToDelete) {
            deletedIndexBlock = i;
            break;
        }
    }

    if (deletedIndexBlock == -1) {
        // Block not found
        return -1;
    }
    else if (deletedIndexBlock == (numDataBlocks - 1)) {
        // Last Block of Array
        free(blockToDelete);
        return 0;
    }

    // Free the Memory
    free(blockToDelete);

    // Shift the Remaining Blocks to replace the deleted block
    int numBlocksToShift = numDataBlocks - deletedIndexBlock - 1;
    while (numBlocksToShift > 0) {
        // Replace the Block
        this->listBlocks[deletedIndexBlock] = this->listBlocks[deletedIndexBlock + 1];
        
        // Update Counters
        deletedIndexBlock++;
        numBlocksToShift--;
    }
    

    // Update the Block Manager
    this->numDataBlocks--;

    // Signify no Issues
    return 0;
}

/**
 * @brief Finding a record with a target key value from all the initialised Data Blocks
 * 
 * @param keyValue 
 * @return Record* 
 */
Record * BlockManager :: findRecord(float keyValue) {
    // Initialise Variables
    int start = 0;
    int end = this->numDataBlocks - 1;
    int curr = 0;
    
    // Use Binary Search to search through the entire list of blocks available
    while (start <= end) {
         // Retrieve first record from the Current Block
        curr = start + (end - start) / 2;
        DataBlock *currBlock = this->listBlocks[curr];
        Record firstRecord = currBlock->records[0];
        Record lastRecord = currBlock->records[currBlock->numRecords - 1];

        // Parse the Records to Float Values so we can compare
        float firstRecordKeyValue = byteToFloat(firstRecord.fgPctHomeBitArray);
        float lastRecordKeyValue = byteToFloat(lastRecord.fgPctHomeBitArray);

        // Check if the target keyValue is within
        if (keyValue > firstRecordKeyValue && keyValue > lastRecordKeyValue) {
            // Curr Block is too Small
            start = curr + 1;
        }
        else if (keyValue < firstRecordKeyValue && keyValue < lastRecordKeyValue) {
            // Curr Block is too Big
            end = curr - 1;
        }
        else if (keyValue > firstRecordKeyValue && keyValue < lastRecordKeyValue) {
            // Record should be within current block
            int numRecords = currBlock->numRecords;

            // Iterate over all the Records to see if we can find
            for (int i{0}; i < numRecords; i += 1) {
                Record currRecord = currBlock->records[i];
                float currRecordKeyValue = byteToFloat(currRecord.fgPctHomeBitArray);

                if (currRecordKeyValue == keyValue) {
                    // Found the First Occurrence Record
                    return &currRecord;
                }
            }

            // Record does not exist
            return NULL;
        }
        else {
            // Should not be possible if ordered
            std::cout << "Error!! Should not be in this loop if ordered sequentially within block." << std::endl;
        }
    }

    // Target Key must have a value larger than the largest or smaller than the smallest in the database
    return NULL;
}


void BlockManager :: shiftRecordsDown(int blockIndex, int recordIndex, int nShift) {

    // To catch invalid input
    if (nShift <= 0 || recordIndex < 0 || blockIndex < 0 || recordIndex > MAX_BLOCK_SIZE) {
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
    Record* mainHoldingArea = (Record*)calloc(nShift, sizeof(Record));
    Record* tempHoldingArea = (Record*)malloc(sizeof(Record)*nShift);

    if (mainHoldingArea == NULL or tempHoldingArea == NULL) {
        std::cout << "Unable to shift records" << std::endl;
        exit(0);
    }

    int remainingSpace = MAX_BLOCK_SIZE - this->listBlocks[blockIndex]->numRecords;
    int toShift;

    // *****TODO Need to account for the case when a new block must be created to account for the shift
    // Im assuming that creation of block is done before shifting, and that shifting will not cause data to go out of bounds

    // While there is no empty space at the end of the block such that I can shift without overflow of records.
    while (remainingSpace < nShift) {
        /*
            The number of records that will be shifted and not pushed out of the current block. 
            If the number of shifts is more than the number of records remaining after the recordIndex, toShift is just set to the MAX_BLOCK_SIZE-recordIndex.
            This is because toShift would become negative otherwise.
        */
        toShift = MAX_BLOCK_SIZE - recordIndex - nShift <= 0 ? MAX_BLOCK_SIZE - recordIndex : MAX_BLOCK_SIZE - recordIndex - nShift;

        // Store the records at the end of the block that will be pushed out by the shifting procedure in tempHoldingArea.
        memcpy(tempHoldingArea, &(this->listBlocks[blockIndex]->records[MAX_BLOCK_SIZE-nShift]), sizeof(Record)*nShift);

        // Do the actual shifting of data within the block itself
        memmove(&(this->listBlocks[blockIndex]->records[recordIndex + nShift]), &(this->listBlocks[blockIndex]->records[recordIndex]), sizeof(Record)*toShift);

        // Write the mainHoldingArea to the front of the block
        memcpy(&(this->listBlocks[blockIndex]->records[recordIndex]), mainHoldingArea, sizeof(Record)*nShift);

        // Since we are moving to new blocks, having an recordIndex no longer makes sense after the first block.
        recordIndex = 0;
        blockIndex++;
        remainingSpace = MAX_BLOCK_SIZE - this->listBlocks[blockIndex]->numRecords;

        // Write the temp to the mainHoldingArea
        memcpy(mainHoldingArea, tempHoldingArea, sizeof(Record)*nShift);
    }

    // One final shift at the last block.
    memmove(&(this->listBlocks[blockIndex]->records[recordIndex + nShift]), &(this->listBlocks[blockIndex]->records[recordIndex]), sizeof(Record)*toShift);

    // Write the main holding area to the front of the block
    memcpy(&(this->listBlocks[blockIndex]->records[0]), mainHoldingArea, sizeof(Record)*nShift);

    free(mainHoldingArea);
    free(tempHoldingArea);
}

void BlockManager :: shiftRecordsUp(int blockIndex, int recordIndex, int nShift) {
    /*
        Assuming that theres no overflow on the block at the top of the chain where the shift is meant to fill gaps,
        and that recordIndex is the starting location of the gap in the original block.

        since shiftUp will only occur if theres a deletion, therefore nShift starting from the recordIndex+nShift within the block should
        not overwrite any existing data.

        *****TO CONFIRM****
    */

    // To catch invalid input
    if (nShift <= 0 || recordIndex < 0 || blockIndex < 0 || recordIndex > MAX_BLOCK_SIZE || this->numDataBlocks - 1 < blockIndex) {
        return;
    }

    Record* mainHoldingArea = (Record*)calloc(nShift, sizeof(Record));
    Record* tempHoldingArea = (Record*)malloc(sizeof(Record)*nShift);

    if (mainHoldingArea == NULL or tempHoldingArea == NULL) {
        std::cout << "Unable to shift records" << std::endl;
        exit(0);
    }

    for (int i = this->numDataBlocks-1; i > blockIndex; i--) {
        
        // Shift the front of the array to the tempHoldingArea before it's overwritten
        memcpy(tempHoldingArea, &(this->listBlocks[i]->records[0]), sizeof(Record)*nShift);

        // Shift the data within the block
        memmove(&(this->listBlocks[i]->records[0]), &(this->listBlocks[i]->records[nShift]), sizeof(Record)*(MAX_BLOCK_SIZE - nShift));

        // Write the mainHoldingArea to the end of the block
        memcpy(&(this->listBlocks[i]->records[MAX_BLOCK_SIZE - nShift]), mainHoldingArea, sizeof(Record)*nShift);

        // Write the temp to the mainHoldingArea
        memcpy(mainHoldingArea, tempHoldingArea, sizeof(Record)*nShift);
    }

    // Number of records remaining at the end of the original block after the gap
    int toShift = MAX_BLOCK_SIZE - recordIndex - nShift;
    // Do the shifting in the main block that has gaps to fill
    memmove(&(this->listBlocks[blockIndex]->records[recordIndex]), &(this->listBlocks[blockIndex]->records[recordIndex - nShift]), sizeof(Record)*toShift);

    // Write the mainHoldingArea to the end of the block
    memcpy(&(this->listBlocks[blockIndex]->records[MAX_BLOCK_SIZE - nShift]), mainHoldingArea, sizeof(Record)*nShift);

    free(mainHoldingArea);
    free(tempHoldingArea);
}