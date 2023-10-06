#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "loadData.h"
#include "./Block Manager/blockManager.h"
#include "./BPlusTree/bPlusTree.h"
#include "./Node/node.h"

using namespace std;

int main() {
    BlockManager blockManager = BlockManager();
    vector<Record> recordBytes = loadData();

    for (int i = 0; i < recordBytes.size(); i++) {
        blockManager.insertRecord(recordBytes[i]);
    }

    blockManager.displayStats();
    return 0;
}