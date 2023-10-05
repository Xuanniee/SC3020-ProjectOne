#ifndef BLOCK_MANAGER
#define BLOCK_MANAGER
// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../loadData.h"
#include <tuple>

// Maximum number of Records in a Data Block {400B - 4B / 20B} should be 19 Blocks, because of block 
// header storing the attribute
#define MAX_RECORDS 19
// Maximum number of Blocks = (approx 27k records/19 records per block)
#define MAX_BLOCKS 1500

/**
 * 20B in total for a single record
 * assuming boolean is 1B
//  */

// typedef struct record{
    
//     //9 bits for all 9 fields (0-9)
//     short int recorderHeader;

//     //index 0 : number of days since epoch
//     short int gameDateEst;

//     //index 1: range is within 4bytes 
//     int teamIdHome;

//     // index 2: range is within 1byte
//     unsigned char ptsHome;

//     //index 3: 3 decimals(0 to 999) + MSB of 1/0
//     unsigned short int fgPctHomeByteArray;

//     //index 4: 3 decimals(0 to 999) + MSB of 1/0
//     unsigned short int ftPctHomeByteArray;

//     //index 5: 3 decimals(0 to 999) + MSB of 1/0
//     unsigned short int fg3PctHomeByteArray;

//     //index 6: range is within 1byte
//     unsigned char astHome;

//     //index 7: range is within 1byt
//     unsigned char rebHome;

//     //index 8: range is within 1byte
//     bool homeTeamWins;
// } Record;


typedef struct dataBlock {
    int numRecords;
    Record records[MAX_RECORDS];
} DataBlock;


class BlockManager {
    private:
        int numDataBlocks = 0;
        int numRecords = 0;
        DataBlock listBlocks[MAX_BLOCKS];
        int MAX_RECORD_INDEX = MAX_RECORDS-1;

    public:
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
         * @brief 
         * 
         * @param blockIndex - the index of the target block
         * @param recordIndex - the index of the target record within the block
         * @param nShift - the number of records to shift by
         * 
         * @return void
         */
        void shiftRecordsUp(int blockIndex, int recordIndex, int nShift);
        /**
         * @brief Insert a record into a sequential file based on value of FG3_PCT_home
         * 
         * @param rec - record to be inserted
        */
        void insertRecord(Record rec);
};

int main();

#endif

