#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>

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

    /**
     * Experiment 1 - Store Data and Report Statistics
     */
    for (int i = 0; i < recordBytes.size(); i++) {
        blockManager.insertRecord(recordBytes[i]);
    }
    
    std::cout << "Experiment 1 Results:" << endl;
    blockManager.displayStats();

    /**
     * Experiment 2 - Build a B+ Tree Index on "FG_PCT_home" and Report Statistics
     */
    BPlusTree bPlusTree = BPlusTree();
    bPlusTree.setRoot(NULL);
    bPlusTree.setHeight(0);
    // Index is always built on FG_PCT_home
    blockManager.buildIndex(&bPlusTree);

    // Determine the Content of the Root Node
    int bPlusTreeHeight = bPlusTree.getHeight();
    // Determine the Number of Nodes in the Tree
    int numNodes = bPlusTree.countNodesInBPlusTree(bPlusTree.getRoot(), bPlusTree.getHeight());
    
    cout << "Experiment 2 Results:" << endl;
    cout << "========= B+ Tree Statistics =========" << endl;
    cout << "Parameter n of the B+ Tree (i.e. numKeys a node can hold): " << NUM_KEYS << endl;
    cout << "Number of Nodes of B+ Tree: " << numNodes << endl;
    cout << "Number of Levels of the B+ Tree: " << bPlusTreeHeight << endl;
    cout << "Content of the Root Node (Only Keys): ";
    bPlusTree.printRootNodeKeys(); 
    cout << "." << endl;

    LeafNode* curr = (LeafNode*) ((InternalNode*) bPlusTree.getRoot())->children[0];

    int cnt = 0;
    while (curr != NULL) {
        for (int i = 0; i < curr->numKeysInserted; i++) {
            cnt++;
            cout << curr->keys[i] << endl;
        }
        curr = curr->next;
    }
    cout << "COUNT " << cnt << endl << endl << endl;

    /**
     * Experiment 3 - Retrieve movies with "FG_PCT_home" equal to 0.5, both inclusively and report the statistics.
     */
    cout << "Experiment 3 Results:" << endl << endl;

    // Index Database Retrieval    
    stack<Node*> myStack;
    Record **recordPtr = NULL;
    auto startIndex = std::chrono::high_resolution_clock::now();
    bPlusTree.findRecordInTree(0.5, &myStack, recordPtr, true);
    auto endIndex = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationNormal = endIndex - startIndex;

    cout << "Running Time of Retrieval Process: " << durationNormal.count() << " seconds" << endl << endl;
    // TODO Specify Method

    // Linear Scan Retrieval
    auto startLinear = std::chrono::high_resolution_clock::now();
    blockManager.linearScanKey(0.500);
    auto endLinear = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationLinear = endLinear - startLinear;

    cout << "Running Time of Retrieval Process (Linear Scan): " << durationLinear.count() << " seconds" << endl << endl << endl;

    /**
     * Experiment 4 - Retrieve those movies with FG_PCT_home from 0.6 to 1, both inclusively and report statistics
     */
    cout << "Experiment 4 Results:" << endl << endl;

    startIndex = std::chrono::high_resolution_clock::now();
    // Find the Start and End Record of the Range Query
    std::pair<Record*, Record*> rangeQueryResult = bPlusTree.findRecordsInRange(0.600, 1);
    Record *startRecord = rangeQueryResult.first;
    Record *endRecord = rangeQueryResult.second;

    // Retrieve a list storing all the actual records
    vector<Record *> actualRecordsList = blockManager.findRecordsInRange(startRecord, endRecord);
    endIndex = std::chrono::high_resolution_clock::now();
    durationNormal = endIndex - startIndex;

    // Time taken to calculate the Average Fg3 is not included in range retrieval
    float sumFG3 = 0.0;
    int numRecs = actualRecordsList.size();
    for (int i = 0; i < numRecs; i += 1) {
        if (actualRecordsList[i]->fg3PctHomeByteArray != 0) {
            sumFG3 += bytesToFloat(actualRecordsList[i]->fg3PctHomeByteArray);
        }
    }
    float avgFG3 = sumFG3 / numRecs;

    cout << "Average of FG3_PCT_home of returned records: " << avgFG3 << endl;
    cout << "Running Time of Retrieval Process: " << durationNormal.count() << " seconds" << endl << endl;
    
    // Linear Scan Retrieval
    startLinear = std::chrono::high_resolution_clock::now();
    blockManager.linearScanRange(0.600, 1);
    endLinear = std::chrono::high_resolution_clock::now();
    durationLinear = endLinear - startLinear;
    cout << "Running Time of Retrieval Process (Linear Scan): " << durationLinear.count() << " seconds" << endl;

    /**
     * Experiment 5 - Delete movies with FG_PCT_home <= 0.35, update B+ Tree and report statistics
     */
    startIndex = std::chrono::high_resolution_clock::now();
    // Delete and Update B+ Tree Function
    endIndex = std::chrono::high_resolution_clock::now();
    durationNormal = endIndex - startIndex;
    
    // Linear Scan Retrieval
    startLinear = std::chrono::high_resolution_clock::now();
    // Insert Linear Scan Method
    blockManager.linearScanRange(0.35, -1, true);
    endLinear = std::chrono::high_resolution_clock::now();
    durationLinear = endLinear - startLinear;
    cout << "Running Time of Retrieval Process (Linear Scan): " << durationLinear.count() << " seconds" << endl;

    // B+ Tree deletion
    blockManager.deleteRange(&bPlusTree, 0, 0.35);

    // Updated Nodes and Height
    bPlusTreeHeight = bPlusTree.getHeight();
    numNodes = bPlusTree.countNodesInBPlusTree(bPlusTree.getRoot(), bPlusTree.getHeight());

    cout << "========= B+ Tree Statistics =========" << endl;
    cout << "Number of Nodes of Updated B+ Tree: " << numNodes << endl;
    cout << "Number of Levels of the Updated B+ Tree: " << bPlusTreeHeight << endl;
    cout << "Content of the Root Node (Only Keys): ";
    bPlusTree.printRootNodeKeys(); 
    cout << "." << endl;
    cout << "Running Time of Retrieval Process: " << durationNormal.count() << " seconds" << endl;
    // Linear will be printed inside the function
    cout << "Number of Data Blocks Accessed (Linear Scan):" << endl;
    cout << "Running Time of Retrieval Process (Linear Scan): " << durationLinear.count() << " seconds" << endl;

    return 0;
}