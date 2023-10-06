// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "blockManager.h"
#include <tuple>

using namespace std;
/**
 * @brief Create a Data Block object
 * 
 * @return DataBlock* 
 */
DataBlock* BlockManager :: createDataBlock() {
    // Since we dont need to allocate space, we can just simulate creation by incrementing numBlocks
    numDataBlocks++;
    listBlocks[numDataBlocks-1].numRecords = 0;
    return &(listBlocks[numDataBlocks-1]);
}


/**
 * @brief Indicates the Datablock to be deleted
 * 
 * @param blockToDelete 
 * @return int 
 */
int BlockManager :: deleteDataBlock(DataBlock *blockToDelete) {
    // Not sure if we still need this function, as we don't really delete blocks anymore
    // Retrieve the Data Block to be deleted
    int numDataBlocks = this->numDataBlocks;
    int deletedIndexBlock = -1;

    // Iterate until we find the position of the block to be deleted
    for (int i=0; i < numDataBlocks; i++) {
        DataBlock *currBlock = &(this->listBlocks[i]);
        if (currBlock == blockToDelete) {
            deletedIndexBlock = i;
            break;
        }
    }

    if (deletedIndexBlock == -1) {
        // Block not found
        return -1;
    }

    // Update the Block Manager
    this->numDataBlocks--;

    // Signify no Issues
    return 0;
}

/**
 * @brief Finding a record with a target key value from all the initialised Data Blocks
 * Returns 2 integers, which corresponds to block index and record index. For insertion, deletion or even when record is not found  
 * 
 * @param keyValue 
 * @return Record* 
 */
std::pair<int,int> BlockManager :: findRecord(float keyValue) {
    if (numRecords == 0) {
        return std::make_pair(0,0);
    }
    // Initialise Index Variables
    int start = 0;
    int end = this->numDataBlocks - 1;
    int curr = 0;

    // Initialise Variables to Return
    int targetBlockIndex = -1;
    int targetRecordIndex = -1;

    // Edge Case: 0 Records in Tree
    if (this->getTotalRecords() == 0) {
        return std::make_pair(0,0);
    }
    
    // Use Binary Search to determine the Index of the Block
    while (start <= end) {
        // Retrieve the Middle Data Block
        curr = start + (end - start) / 2;
        DataBlock *currBlock = &(listBlocks[curr]);

        // Retrieve the first and last records from the Middle (Current) Block
        Record firstRecord = currBlock->records[0];
        Record lastRecord = currBlock->records[currBlock->numRecords - 1];

        // Parse the Key Values of Records to Float Values for Comparison
        float firstRecordKeyValue = bytesToFloat(firstRecord.fgPctHomeByteArray);
        float lastRecordKeyValue = bytesToFloat(lastRecord.fgPctHomeByteArray);

        // Check if the target keyValue is within the Current Block
        if (keyValue > firstRecordKeyValue && keyValue > lastRecordKeyValue) {
            // Curr Block is too Small, Search the Bigger Half
            start = curr + 1;
        }
        else if (keyValue < firstRecordKeyValue && keyValue < lastRecordKeyValue) {
            // Curr Block is too Big, Search the Smaller half
            end = curr - 1;
        }
        else if (keyValue >= firstRecordKeyValue && keyValue <= lastRecordKeyValue) {
            // Found Target Block, record the index
            targetBlockIndex = curr;

            // Iterate over all the Records to check if target Record is present
            int numRecords = currBlock->numRecords;
            for (int i=0; i < numRecords; i += 1) {
                Record currRecord = currBlock->records[i];
                float currRecordKeyValue = bytesToFloat(currRecord.fgPctHomeByteArray);

                if (currRecordKeyValue == keyValue) {
                    // Found the First Occurrence of Target Record
                    targetRecordIndex = i;

                    // Case 1: First Target Record is found (Selection or Deletion)
                    return std::make_pair(targetBlockIndex, targetRecordIndex);
                }
            }

            // Non-Existent Record
            int insertionIndex = 0;

            // Determine where to place the record to be inserted
            for (int i=0; i < numRecords; i += 1) {
                // Retrieve the Record
                Record currRecord = currBlock->records[i];
                float currRecordKeyValue = bytesToFloat(currRecord.fgPctHomeByteArray);

                // Find the first record whose keyValue is larger
                if (currRecordKeyValue > keyValue) {
                    targetRecordIndex = i;

                    // Case 2: Insertion Record's Index found
                    return std::make_pair(targetBlockIndex, targetRecordIndex);
                }
            }

            // Should not have a case where the target index is larger than all available indexes
        }
        else {
            // Should not be possible if ordered
            std::cout << keyValue << " " << firstRecordKeyValue << " " << lastRecord.fgPctHomeByteArray << std::endl;
            std::cout << currBlock->numRecords << endl;
            std::cout << "Error!! Should not be in this loop if ordered sequentially within block." << std::endl;
        }
    }

    // Case 3: Target Key is the largest or smallest in the database
    // Retrieve the Smallest Record
    DataBlock *smallestDataBlock = &(this->listBlocks[0]);
    Record smallestRecord = smallestDataBlock->records[0];

    // Evaluate if the target to be inserted is smaller
    float smallestRecordKeyValue = bytesToFloat(smallestRecord.fgPctHomeByteArray);
    // If it is a duplicate key, it will take the smallest
    if (keyValue <= smallestRecordKeyValue) {
        return std::make_pair(0, 0);
    }
    else if (keyValue == smallestRecordKeyValue) {
        std::cout << "Equals current smallest record" << std::endl;
    }

    // Repeat for Largest Record
    DataBlock *largestDataBlock = &(this->listBlocks[this->numDataBlocks - 1]);
    Record largestRecord = largestDataBlock->records[largestDataBlock->numRecords - 1];

    float largestRecordKeyValue = bytesToFloat(largestRecord.fgPctHomeByteArray);
    // If it is a duplicate key, it will take the largest
    if (keyValue >= largestRecordKeyValue) {
        return std::make_pair((this->numDataBlocks - 1), (largestDataBlock->numRecords));
    }
    else if (keyValue == largestRecordKeyValue) {
        std::cout << "Equals current largest record" << std::endl;
    }

    // Error
    std::cout << "Error!! Most likely inserted record == smallest or largest as not handled yet" << std::endl;
    return std::make_pair(-1, -1);
}


void BlockManager :: shiftRecordsDown(int blockIndex, int recordIndex, int nShift) {

    // To catch invalid input
    if (nShift <= 0 || recordIndex < 0 || blockIndex < 0 || recordIndex > MAX_RECORD_INDEX) {
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

    if (mainHoldingArea == NULL || tempHoldingArea == NULL) {
        std::cout << "Unable to shift records" << std::endl;
        exit(0);
    }

    int remainingSpace = MAX_RECORD_INDEX - this->listBlocks[blockIndex].numRecords;
    int toShift = MAX_RECORD_INDEX - nShift;

    // *****TODO Need to account for the case when a new block must be created to account for the shift
    // Im assuming that creation of block is done before shifting, and that shifting will not cause data to go out of bounds

    // While there is no empty space at the end of the block such that I can shift without overflow of records.
    while (remainingSpace < nShift) {
        /*
            The number of records that will be shifted and not pushed out of the current block. 
            If the number of shifts is more than the number of records remaining after the recordIndex, toShift is just set to the MAX_RECORD_INDEX-recordIndex.
            This is because toShift would become negative otherwise.
        */
        toShift = MAX_RECORD_INDEX - recordIndex - nShift <= 0 ? 0 : MAX_RECORD_INDEX - recordIndex - nShift;

        // Store the records at the end of the block that will be pushed out by the shifting procedure in tempHoldingArea.
        memcpy(tempHoldingArea, &(this->listBlocks[blockIndex].records[MAX_RECORD_INDEX-nShift]), sizeof(Record)*nShift);

        if (toShift != 0) {
            // Do the actual shifting of data within the block itself
            memmove(&(this->listBlocks[blockIndex].records[recordIndex + nShift]), &(this->listBlocks[blockIndex].records[recordIndex]), sizeof(Record)*toShift);
        }   
        // Write the mainHoldingArea to the front of the block
        memcpy(&(this->listBlocks[blockIndex].records[recordIndex]), mainHoldingArea, sizeof(Record)*nShift);

        // Since we are moving to new blocks, having an recordIndex no longer makes sense after the first block.
        recordIndex = 0;
        blockIndex++;
        remainingSpace = MAX_RECORD_INDEX - this->listBlocks[blockIndex].numRecords;

        // Write the temp to the mainHoldingArea
        memcpy(mainHoldingArea, tempHoldingArea, sizeof(Record)*nShift);
    }

    // One final shift at the last block.
    memmove(&(this->listBlocks[blockIndex].records[recordIndex + nShift]), &(this->listBlocks[blockIndex].records[recordIndex]), sizeof(Record)*toShift);

    // Write the main holding area to the front of the block
    memcpy(&(this->listBlocks[blockIndex].records[0]), mainHoldingArea, sizeof(Record)*nShift);

    free(mainHoldingArea);
    free(tempHoldingArea);
}

void BlockManager :: insertRecord(Record rec) {
    int ib, ir;
    float pk = bytesToFloat(rec.fgPctHomeByteArray);

    std::tie(ib, ir) = findRecord(pk);

    // create new data block if all blocks are full
    if (numDataBlocks == 0 || listBlocks[numDataBlocks-1].numRecords == MAX_RECORDS) createDataBlock(); 
    

    ib += ir / MAX_RECORDS;
    ir %= MAX_RECORDS;

    shiftRecordsDown(ib, ir, 1);
    listBlocks[ib].records[ir] = rec;
    numRecords++;
    listBlocks[numDataBlocks-1].numRecords++;
}

void BlockManager :: buildIndex(BPlusTree* btree) {
    if (btree->getRoot() == NULL) {
        btree->setRoot((Node*)malloc(sizeof(Node)));
    }

    for (int i = 0; i < numDataBlocks; i++) {
        for (int j = 0; j < listBlocks[i].numRecords; j++) {
            btree->insertKeyInTree(bytesToFloat(listBlocks[i].records[j].fgPctHomeByteArray), &listBlocks[i].records[j]);
        }
    }
}

void BlockManager :: displayStats() {
    cout << "=========Block Manager statistics=========" << endl;
    cout << "Total number of records stored: " << numRecords << endl;
    cout << "Size of each record: " << sizeof(Record) << endl;
    cout << "Number of records stored in each block: " << MAX_RECORDS << endl;
    cout << "Number of blocks for storing data: " << numDataBlocks << endl;
}