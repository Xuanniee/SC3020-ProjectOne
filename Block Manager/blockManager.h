// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Maximum number of Records in a Data Block {400B / 23B} shud be 17, but I use 10 to prevent memory issues for now
#define MAX_RECORDS 10
// Maximum number of Blocks (500MB / 400B)
#define MAX_BLOCKS 1250000

typedef struct record {
    // Record Header is a bit array of 9 bits to correspond to each of the columns
    unsigned char *recordHeader;

    // Field Headers for the 9 Attributes
    // Format will be their size and the Pointers to their location
    size_t gameDateEstSize = 10;
    unsigned char *gameDateEstBitArray;

    size_t teamIdHomeSize = (31 + 7) / 8;   // Function to round up to the number of bytes required from 31 bits
    unsigned char *teamIdHomeBitArray;

    size_t ptsHomeSize = 1;
    unsigned char *ptsHomeBitArray;

    // Primary Key; Float
    size_t fgPctHomeSize = (11 + 7) / 8;    // Requires 11 bits
    unsigned char *fgPctHomeBitArray;

    size_t ftPctHomeSize = (11 + 7) / 8;    // Requires 11 bits
    unsigned char *ftPctHomeBitArray;

    size_t fg3PctHomeSize = (11 + 7) / 8;    // Requires 11 bits
    unsigned char *fg3PctHomeBitArray;

    size_t astHomeSize = 1;
    unsigned char *astHomeBitArray;

    size_t rebHomeSize = 1;
    unsigned char *rebHomeBitArray;

    bool homeTeamWins;
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

typedef struct blockManager {
    // Attributes
    int numDataBlocks = 0;
    int numRecords = 0;
    DataBlock *listBlocks[MAX_BLOCKS];     // Array of DataBlocks

} BlockManager;