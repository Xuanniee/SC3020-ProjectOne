// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <./blockManager.h>

/**
 * Helper Functions from the rest to prevent syntax errors
 */
float byteToFloat(unsigned char *recordKeyBitArray) {}

/**
 * @brief Create a Data Block object
 * 
 * @return void* 
 */
DataBlock* createDataBlock(BlockManager& blockManager) {
    // Fix the Size of the Data Block to 400B
    size_t dataBlockSize = 400;

    // Malloc 400 Bytes of Space for the Data Block
    DataBlock *newDataBlock = (DataBlock*) malloc(dataBlockSize);

    // Verify Memory Allocation was successful
    if (newDataBlock == NULL) {
        std::cout << "Memory Allocation Failed." << std::endl;
    }
    else {
        std::cout << "Memory Allocation Successful. One Data Block of 400B has been initialised." << std::endl;
    }

    // Add to the Block Manager's Array of Blocks, not Records since it should be handled by insertRecord
    blockManager.listBlocks[blockManager.numDataBlocks] = newDataBlock;
    blockManager.numDataBlocks += 1;

    return newDataBlock;
}


int deleteDataBlock(DataBlock *blockToDelete, BlockManager& blockManager) {
    // Retrieve the Data Block to be deleted
    int numDataBlocks = blockManager.numDataBlocks;
    int deletedIndexBlock = -1;

    // Iterate until we find the position of the block to be deleted
    for (int i{0}; i < numDataBlocks; i += 1) {
        DataBlock *currBlock = blockManager.listBlocks[i];
        if (currBlock == blockToDelete) {
            deletedIndexBlock = i;
            break;
        }
    }

    if (deletedIndexBlock == -1) {
        // Block not found
        return -1;
    }

    // Free the Memory
    free(blockToDelete);

    // Shift the Remaining Blocks to replace the deleted block
    int numBlocksToShift = numDataBlocks - deletedIndexBlock - 1;
    while (numBlocksToShift > 0) {
        // Replace the Block
        blockManager.listBlocks[deletedIndexBlock] = blockManager.listBlocks[deletedIndexBlock + 1];
        
        // Update Counters
        deletedIndexBlock += 1;
        numBlocksToShift -= 1;
    }
    

    // Update the Block Manager
    blockManager.numDataBlocks -= 1;

    // Signify no Issues
    return 0;
}

/**
 * @brief Finding a record with a target key value from all the initialised Data Blocks
 * 
 * @param keyValue 
 * @return Record* 
 */
Record *findRecord(float keyValue, BlockManager& blockManager) {
    // Initialise Variables
    int start = 0;
    int end = blockManager.numDataBlocks - 1;
    int curr = 0;
    
    // Use Binary Seary to search through the entire list of blocks available
    while (start <= end) {
         // Retrieve first record from the Current Block
        curr = start + (end - start) / 2;
        DataBlock *currBlock = blockManager.listBlocks[curr];
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


