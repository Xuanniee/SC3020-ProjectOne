#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "loadData.h"
#include "./Block Manager/blockManager.h"
#include "./BPlusTree/bPlusTree.h"
#include "./Node/node.h"

using namespace std;

int main() {
    /*
    ============== INSERTING RECORDS TO BLOCK MANAGER ==============
            ========= Block Manager statistics =========
            Total number of records stored: 26651
            Size of each record: 20
            Number of records stored in each block: 19
            Number of blocks for storing data: 1403
    */
    BlockManager blockManager = BlockManager();
    vector<Record> recordBytes = loadData();

    for (int i = 0; i < recordBytes.size(); i++) {
        blockManager.insertRecord(recordBytes[i]);
    }

    blockManager.displayStats();
    return 0;
}