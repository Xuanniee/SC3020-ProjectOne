#ifndef BLOCK_MANAGER
#define BLOCK_MANAGER
// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../Record/record.h"
#include <tuple>
#include "../loadData.h"
#include "../BPlusTree/bPlusTree.h"
#include "../Node/node.h"


// Maximum number of Records in a Data Block {400B - 4B / 20B} should be 19, because of block 
// header storing the attribute
#define MAX_RECORDS 19
// Maximum number of Blocks = (approx 27k records/19 records per block)
#define MAX_BLOCKS 1500
#define MAX_RECORD_INDEX (MAX_RECORDS-1)

/**
 * 20B in total for a single record
 * assuming boolean is 1B
//  */

typedef struct dataBlock {
    int numRecords;
    Record records[MAX_RECORDS];
} DataBlock;


class BlockManager {
    private:
        int numDataBlocks;
        int numRecords;
        DataBlock listBlocks[MAX_BLOCKS];

    public:
        BlockManager() {
            numDataBlocks = 0;
            numRecords = 0;
        }

        DataBlock* getListBlocks() {
            return this->listBlocks;
        };

        int getNumBlocks() {
            return this->numDataBlocks;
        }

        int getTotalRecords() {
            return this->numRecords;
        }

        /**
         * @brief Create a Data Block object
         * 
         * @return DataBlock* 
         */
        DataBlock* createDataBlock();
        /**
         * @brief Delete a Data Block object
         * 
         * @return int
         */
        int deleteDataBlock(DataBlock *blockToDelete);
        /**
         * @brief find a record given a keyValue, or the appropriate place to insert a record with the given key.
         * 
         * @return Record*
         */
        std::pair<int,int> findRecord(float keyValue);
        /**
         * @brief 
         * 
         * @param blockIndex - the index of the target block
         * @param recordIndex - the index of the target record within the block
         * @param nShift - the number of records to shift by
         * 
         * @return void
         */
        void shiftRecordsDown(int blockIndex, int offset, int nShift);
        /**
         * @brief Insert a record into a sequential file based on value of FG3_PCT_home
         * 
         * @param rec - record to be inserted
        */
        void insertRecord(Record rec);

        /**
         * @brief Build a B+ Tree index on the stored records
         * 
         */
        void buildIndex(BPlusTree* btree);

        /**
         * @brief Display statistics about the blockManager
         * 
         */
        void displayStats();
};

#endif

