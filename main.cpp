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
    BPlusTree *bPlusTreePtr = (BPlusTree*) malloc(sizeof(BPlusTree));
    // Index is always built on FG_PCT_home
    blockManager.buildIndex(bPlusTreePtr);

    // Determine the Content of the Root Node
    int bPlusTreeHeight = bPlusTreePtr->getHeight();

    // Determine the Number of Nodes in the Tree
    int numNodes = bPlusTreePtr->countNodesInBPlusTree(bPlusTreePtr->getRoot());
    
    cout << "Experiment 2 Results:" << endl;
    cout << "========= B+ Tree Statistics =========" << endl;
    cout << "Parameter n of the B+ Tree (i.e. numKeys a node can hold): " << NUM_KEYS << endl;
    cout << "Number of Nodes of B+ Tree: " << numNodes << endl;
    cout << "Number of Levels of the B+ Tree: " << bPlusTreeHeight << endl;
    cout << "Content of the Root Node (Only Keys): ";
    bPlusTreePtr->printRootNodeKeys(); 
    cout << "." << endl;

    /**
     * Experiment 3 - Retrieve movies with "FG_PCT_home" equal to 0.5, both inclusively and report the statistics.
     */
    // Index Database Retrieval    
    stack<Node*> *myStackPtr;
    Record **recordPtr = NULL;
    auto startIndex = std::chrono::high_resolution_clock::now();
    bPlusTreePtr->findRecordInTree(0.5, myStackPtr, recordPtr);
    auto endIndex = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationNormal = endIndex - startIndex;
    
    // Linear Scan Retrieval
    auto startLinear = std::chrono::high_resolution_clock::now();
    blockManager.linearScanKey(0.500);
    // Insert Linear Scan Method
    auto endLinear = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationLinear = endLinear - startLinear;
    std::cout << "Running Time of Retrieval Process (Linear Scan): " << durationLinear.count() << " seconds" <<endl;


    cout << "Experiment 3 Results:" << endl;
    
    cout << "Number of Data Blocks Accessed:" << endl;
    cout << "Average of FG_PCT_home records returned:" << endl;
    cout << "Running Time of Retrieval Process: " << durationNormal.count() << " seconds" << endl;
    // TODO Specify Method

    // Linear will be printed inside the function
    // cout << "Number of Data Blocks Accessed (Linear Scan):" << endl;
    // cout << "Running Time of Retrieval Process (Linear Scan): " << durationLinear.count() << " seconds" << endl;

    /**
     * Experiment 4 - Retrieve those movies with FG_PCT_home from 0.6 to 1, both inclusively and report statistics
     */
    startIndex = std::chrono::high_resolution_clock::now();
    // Insert Ranged Query Method
    endIndex = std::chrono::high_resolution_clock::now();
    durationNormal = endIndex - startIndex;
    
    // Linear Scan Retrieval
    startLinear = std::chrono::high_resolution_clock::now();
    // Insert Linear Scan Method
    blockManager.linearScanRange(0.600, 1);
    endLinear = std::chrono::high_resolution_clock::now();
    durationLinear = endLinear - startLinear;
    cout << "Running Time of Retrieval Process (Linear Scan): " << durationLinear.count() << " seconds" << endl;
    
    
    cout << "Experiment 4 Results:" << endl;
    cout << "Number of Index Nodes Accessed:" << endl;
    cout << "Number of Data Blocks Accessed:" << endl;
    cout << "Average of FG3_PCT_home of returned records:" << endl;
    cout << "Running Time of Retrieval Process: " << durationNormal.count() << " seconds" << endl;

    // Linear will be printed inside the function
    // cout << "Number of Data Blocks Accessed (Linear Scan):" << endl;
    // cout << "Running Time of Retrieval Process (Linear Scan): " << durationLinear.count() << " seconds" << endl;

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


    // Updated Nodes and Height
    bPlusTreeHeight = bPlusTreePtr->getHeight();
    numNodes = bPlusTreePtr->countNodesInBPlusTree(bPlusTreePtr->getRoot());

    cout << "========= B+ Tree Statistics =========" << endl;
    cout << "Number of Nodes of Updated B+ Tree: " << numNodes << endl;
    cout << "Number of Levels of the Updated B+ Tree: " << bPlusTreeHeight << endl;
    cout << "Content of the Root Node (Only Keys): ";
    bPlusTreePtr->printRootNodeKeys(); 
    cout << "." << endl;
    cout << "Running Time of Retrieval Process: " << durationNormal.count() << " seconds" << endl;
    // Linear will be printed inside the function
    // cout << "Number of Data Blocks Accessed (Linear Scan):" << endl;
    // cout << "Running Time of Retrieval Process (Linear Scan): " << durationLinear.count() << " seconds" << endl;

    return 0;
}