#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "loadData.h"
#include "./Block Manager/blockManager.h"
#include "./BPlusTree/bPlusTree.h"
#include "./Node/node.h"

using namespace std;

int main() {
    // Initialise the Variables and Objects
    BlockManager blockManager = BlockManager();
    vector<Record> recordBytes = loadData();

    /**
     * Experiment 1 - Store Data and Report Statistics
     */
    for (int i = 0; i < recordBytes.size(); i++) {
        blockManager.insertRecord(recordBytes[i]);
    }
    
    cout << "Experiment 1 Results:" << endl;
    blockManager.displayStats();

    /**
     * Experiment 2 - Build a B+ Tree Index on "FG_PCT_home" and Report Statistics
     */
    BPlusTree *bPlusTreePtr = NULL;
    // Index is always built on FG_PCT_home
    blockManager.buildIndex(bPlusTreePtr);

    // cout << "Experiment 2 Results:" << endl;
    // cout << "========= B+ Tree Statistics =========" << endl;
    // cout << "Parameter n of the B+ Tree (i.e. numKeys a node can hold): " << bPlusTreePtr->getRoot << endl;
    // cout << "Size of each record: " << sizeof(Record) << endl;
    // cout << "Number of records stored in each block: " << MAX_RECORDS << endl;
    // cout << "Number of blocks for storing data: " << numDataBlocks << endl;

    return 0;
}