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
        int invalidRecords;

        /**
         * @brief find a record given a keyValue, or the appropriate place to insert a record with the given key.
         * 
         * @return Record*
         */
        std::pair<int,int> findRecordBinarySearch(float keyValue);

    public:
        BlockManager() {
            numDataBlocks = 0;
            numRecords = 0;
            invalidRecords =0;
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

        int getInvalidRecords(){
            return this->invalidRecords;
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

        /**
         * @brief Delete range of keys, inclusive of both upper & lower limits
         * 
         * @param btree B+ Tree index
         * @param low Lower key limit
         * @param upp Upper key limit
         */
       void deleteRange(BPlusTree* btree, float low, float upp);

       /**
        * @brief Get all records in range
        * 
        * @param low
        * @param upp
        * 
        * @return returns a list of record pointers
        */
       std::vector<Record*> findRecordsInRange(Record* low, Record* upp);

       /**
        * @brief Get the block number given an address
        * 
        * @param address
        * 
        * @return returns a pair (block index, relative record index)
        */
       std::pair<int, int> getBlockFromAddress(Record* address);
        
        /** @brief linear search of records for single key query
         * 
         * @param keyValue1 key value to be searched
         */
       void linearScanKey(float keyValue1);

        /**
         * @brief linear search of records for range key query
         * 
         * @param keyValue1 lower limit 
         * @param keyValue2 upper limit if applicable
         * @param lower_than if true, search will be lower or equal to keyValue1, else, search will be higher or equal to keyValue2 
         */
       void linearScanRange(float keyValue1,  float keyValue2 = -1, bool lower_than = false);

};

#endif