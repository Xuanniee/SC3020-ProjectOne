#ifndef BLOCK_MANAGER
#define BLOCK_MANAGER
// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Maximum number of Records in a Data Block {400B - 4B / 20B} should be 19 Blocks, because of block 
// header storing the attribute
#define MAX_RECORDS 19
// Maximum number of Blocks = (approx 27k records/19 records per block)
#define MAX_BLOCKS 1500

/**
 * 20B in total for a single record
 * assuming boolean is 1B
 */
typedef struct record {
    // Record Header is a bit array of 9 bits to correspond to each of the columns
    short int recorderHeader;

    // Field Headers for the 9 Attributes
    // First 2 attributes placed on top to reduct padding size
    bool homeTeamWins;

    // 3B is enough to represents all the dates we need but 4B for ease
    int gameDateEst; 

    // Similar format to gameDateEst, so use same data size
    int teamIdHome; 

    // Integer by nature of data
    unsigned char ptsHome;

    // Primary Key; Float; Uses 2B
    unsigned char fgPctHomeByteArray[2];

    // Same reason as above
    unsigned char ftPctHomeByteArray[2];

    // Same reason as above
    unsigned char fg3PctHomeByteArray[2];

    // Same as ptsHome
    unsigned char astHome;
} Record;


typedef struct dataBlock {
    int numRecords;
    Record records[MAX_RECORDS];

    // I don't think we can use hashmap since it is unordered
    // struct hashmap {
    //     float key;
    //     Record *value;  // Pointer to the actual record
    // } map[MAX_RECORDS];

} DataBlock;

// typedef struct blockManager {
//     // Attributes
//     int numDataBlocks = 0;
//     int numRecords = 0;
//     DataBlock *listBlocks[MAX_BLOCKS];     // Array of DataBlocks

// } BlockManager;

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
#endif