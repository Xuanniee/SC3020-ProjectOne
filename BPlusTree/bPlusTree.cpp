// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <stack>
#include "bPlusTree.h"

/**
 * @brief Helper Function for insertKeyInTree(). Needs a way to insert new keys or records
 * into the index while maintaining the sorted order.
 * 
 * @param key 
 * @param targetRecord 
 * @return int 
 */
int insertionSortInsertLeafNode(float key, Record *targetRecord, int numKeysInTarget, LeafNode *targetLeafNode) {
    int currKey = key;
    Record *currRecord = targetRecord;

    // Iterate over the array to compare the values of the key
    for (int i {0}; i < numKeysInTarget; i += 1) {
        // Current Key Value is Larger should be swapped.
        if (targetLeafNode->keys[i] > currKey) {
            // Swap the Keys First
            int tmpKey = currKey;
            currKey = targetLeafNode->keys[i];
            targetLeafNode->keys[i] = tmpKey;

            // Swap the Records Next
            Record *tmpRecord = currRecord;
            currRecord = targetLeafNode->records[i];
            targetLeafNode->records[i] = tmpRecord;
        }
    }

    // After the Loop, the curr values are the largest
    // Insert the Key into the Array in the Leaf Node
    targetLeafNode->keys[numKeysInTarget] = currKey;

    // Insert the Record into the Ptr
    targetLeafNode->records[numKeysInTarget] = currRecord;
    
    // No issues
    return 0;
}

/**
 * @brief Same Insertion Sort Method as above. However built to allow sorting for arrays
 * with keys of size (n+1).
 * 
 * @param key 
 * @param targetRecord 
 * @param numKeysInTarget 
 * @param combinedKeysArray 
 * @param combinedRecordsArray 
 * @return std::pair<float*, Record**> 
 */
std::pair<float*, Record**> insertionSortInsertArray(float key, Record *targetRecord, int numKeysInTarget, float combinedKeysArray[]
, Record *combinedRecordsArray[]) {
    int currKey = key;
    Record *currRecord = targetRecord;

    // Iterate over the array to compare the values of the key
    for (int i {0}; i < numKeysInTarget; i += 1) {
        // Current Key Value is Larger should be swapped.
        if (combinedKeysArray[i] > currKey) {
            // Swap the Keys First
            int tmpKey = currKey;
            currKey = combinedKeysArray[i];
            combinedKeysArray[i] = tmpKey;

            // Swap the Records Next
            Record *tmpRecord = currRecord;
            currRecord = combinedRecordsArray[i];
            combinedRecordsArray[i] = tmpRecord;
        }
    }

    // After the Loop, the curr values are the largest
    combinedKeysArray[numKeysInTarget] = currKey;
    combinedRecordsArray[numKeysInTarget] = currRecord;
    
    // No issues
    return std::make_pair(combinedKeysArray, combinedRecordsArray);
}

/**
 * @brief Updates the Key for Insertion of Key into indexes.
 * Target record must be already on disk
 * 
 * @param key 
 * @param targetRecord 
 * @return 0 if there is nothing wrong 
 */
int BPlusTree::insertKeyInTree(float key, Record* targetRecord) {
    // Retrieve attributes of the B+ Tree
    int heightTree = this->getHeight();
    Node *BPlusTreeRoot = this->getRoot();

    // Search the Tree to see if the B+ tree contains the Record Key
    std::pair<bool, LeafNode*> searchResult = findRecordInTree(key);

    // Check if the Key Value exists in the B+ Tree
    if (searchResult.first) {
        // Record exists, do nothing
        return 0;
    }

    // If the key does not exists in tree, check if there is space
    LeafNode *targetLeafNode = searchResult.second;
    int numKeysInTarget = targetLeafNode->numKeysInserted;
    int numRecordsInTarget = targetLeafNode->numRecordsInserted;

    // Case 1 - Leaf Node has space to directly insert
    if (numKeysInTarget < NUM_KEYS) {
        // Insert the Record and Key
        insertionSortInsertLeafNode(key, targetRecord, numKeysInTarget, targetLeafNode);

        // Update the numkeys and numRecords
        targetLeafNode->numKeysInserted += 1;
        targetLeafNode->numRecordsInserted += 1;
    }

    // Case 2 - Leaf Node is full, has (n+1) keys now. 
    // Create a new node
    LeafNode *newLeafNode = (LeafNode*) malloc(sizeof(LeafNode));
    newLeafNode->next = NULL;

    // Distribute the keys - MUST ALWAYS BE 40 else, sth is wrong
    if ((numKeysInTarget + 1) % 2 == 0) {
        // Even number of keys. Create a New Array to store all the  (n + 1) keys
        float combinedKeysArray[NUM_KEYS + 1];
        Record *combinedRecordsArray[NUM_KEYS + 1];
        for (int i{0}; i < numKeysInTarget; i += 1) {
            combinedKeysArray[i] = targetLeafNode->keys[i];
            combinedRecordsArray[i] = targetLeafNode->records[i];
        }

        // Insert the n+1th key. Okay to pass by value since don't need the arrays afterwards
        std::pair<float*, Record**> insertionResult = insertionSortInsertArray(key, targetRecord, numKeysInTarget, combinedKeysArray, combinedRecordsArray);
        float *insertionKeysArray = insertionResult.first;
        Record **insertionRecordsArray = insertionResult.second;
        
        // Start copying from the index == numKeys/2
        int numKeysToCopy = numKeysInTarget / 2;

        // Update the Old Leaf Node
        for (int i{0}; i < numKeysToCopy; i += 1) {
            // Copy the Keys over
            targetLeafNode->keys[i] = *(insertionKeysArray + i);

            // Copy the Record over
            targetLeafNode->records[i] = *(insertionRecordsArray + i);
        }

        // Update the New Leaf Node
        int counter = 0;
        for (int i{numKeysToCopy}; i < numKeysInTarget; i += 1) {
            // New leaf should have no records, so start from 0
            newLeafNode->keys[counter] = *(insertionKeysArray + i);
            newLeafNode->records[counter] = *(insertionRecordsArray + i);

            counter += 1;
        }

        // Update the number of keys inserted in the nodes
        targetLeafNode->numKeysInserted = numKeysToCopy;
        targetLeafNode->numRecordsInserted = numKeysToCopy;
        newLeafNode->numKeysInserted = numKeysToCopy;
        newLeafNode->numRecordsInserted = numKeysToCopy;

        // Update the nextLeafNode Ptr, which might point to NULL
        LeafNode *nextLeafNode = targetLeafNode->next;
        targetLeafNode->next = newLeafNode;
        newLeafNode->next = nextLeafNode;
    }
    else {
        // Odd Number of Keys. Original node has 1 more key and record
        // Nth happens since we will always have 40 Keys when we split
        // Should remove the if clause. But I left it here so that I will rmb what happens and can explain
    }

    // Update the Parent Node
    InternalNode *parentNode = targetLeafNode->parent;

    bool reachRoot = false;
    do {
        // If Parent Does not exist, i.e. currently height is 1
        if (parentNode == NULL) {
            // Create a New Parent Node
            InternalNode *newParentNode = (InternalNode*) malloc(sizeof(InternalNode));
            newParentNode->numChildrenNodes = 2;
            newParentNode->numKeysInserted = 1;
            newParentNode->parent = NULL;

            // Update the TWO Children Nodes. Old is Smaller
            newParentNode->children[0] = targetLeafNode;
            newParentNode->children[1] = newLeafNode;

            // Insert the ONE key in parent, smallest value in RST
            newParentNode->keys[0] = newLeafNode->keys[0];
        
            // Update the Height of the Tree
            this->height += 1;

            // Update the Root of the Tree
            this->root = newParentNode;

            // End Method, Return 0 for no issues
            reachRoot = true;
        }
        // Parent Node is not Full
        else if (parentNode->numKeysInserted < NUM_KEYS) {
            // Update the Parent Node Directly
            InternalNode *existingParentNode = targetLeafNode->parent;
            
            // New Key to be inserted will be the largest value since parent keys are already sorted
            float newKey = newLeafNode->keys[0];
            // Cannot update the numKeys until we insert the new key
            existingParentNode->keys[existingParentNode->numKeysInserted] = newKey;
            existingParentNode->children[existingParentNode->numChildrenNodes] = newLeafNode;

            // Update the Number of Keys Inserted
            existingParentNode->numKeysInserted += 1;
            existingParentNode->numChildrenNodes += 1;

            // Don;t need update parent, end process
            reachRoot = true;
        }
        // Parent Node is Full
        else if (parentNode->numKeysInserted >= NUM_KEYS) {
            /**
             * @brief The keys in the parent nodes are already sorted.
             * So the new key-child pair will be the largest and to the
             * right.
             */

            // Min Internal Nodes: Floor(n/2) = 19 when n is 39
            int numKeysLeftParent;
            int numKeysRightParent;

            // Determine the Number of Keys each node should have
            if ((parentNode->numKeysInserted+1) % 2 == 0) {
                // Even no. of nodes, so split equally
                numKeysLeftParent = (parentNode->numKeysInserted + 1) / 2;
                numKeysRightParent = (parentNode->numKeysInserted + 1) / 2;
            }
            else {
                // Odd no. of nodes, left child takes 1 more
                numKeysLeftParent = std::floor((parentNode->numKeysInserted + 1) / 2) + 1;
                numKeysRightParent = std::floor((parentNode->numKeysInserted + 1) / 2);
            }

            // Create a Sibling Node
            InternalNode *newSiblingNode = (InternalNode*) malloc(sizeof(InternalNode));

            // Copy over the Keys and Children of original node to the sibling node
            int counter = 0;
            for (int i{numKeysLeftParent}; i < (parentNode->numKeysInserted); i += 1) {
                // Sibling Node starts from 0, while Parent Node starts from Middle cos larger
                newSiblingNode->keys[counter] = parentNode->keys[i];
                newSiblingNode->children[counter] = parentNode->children[i];

                counter += 1;
            }
            // Add the Last Key to the Rightmost Sibling Node {Smallest Key of New Leaf Node}
            newSiblingNode->keys[counter] = newLeafNode->keys[0];
            newSiblingNode->children[counter] = newLeafNode;

            // Have the Last Pointer point to NULL
            newSiblingNode->children[NUM_KEYS-1] = NULL;

            // Update Attributes for Sibling Node
            newSiblingNode->numChildrenNodes = numKeysRightParent;
            newSiblingNode->numKeysInserted = numKeysRightParent;

            // Update Attributes for Original parent Node
            parentNode->numChildrenNodes = numKeysLeftParent;
            parentNode->numKeysInserted = numKeysLeftParent;

            // Parent Node to Point to sibling Node
            parentNode->children[NUM_KEYS-1] = newSiblingNode;

            // Update for Grandparent Node Recursively
            InternalNode *grandparentNode = parentNode->parent;
            
            // Remember Parent is Smaller than its sibling
            // reachRoot still false
        }
    } while (!reachRoot);

    // No issues    
    return 0;
}

// int updateParentNode(InternalNode *parentNode, LeafNode *targetLeafNode, LeafNode *newLeafNode) {
//     // If Parent Does not exist, i.e. currently height is 1
//     if (parentNode == NULL) {
//         // Create a New Parent Node
//         InternalNode *newParentNode = (InternalNode*) malloc(sizeof(InternalNode));
//         newParentNode->numChildrenNodes = 2;
//         newParentNode->numKeysInserted = 1;
//         newParentNode->parent = NULL;

//         // Update the TWO Children Nodes. Old is Smaller
//         newParentNode->children[0] = targetLeafNode;
//         newParentNode->children[1] = newLeafNode;

//         // Insert the ONE key in parent, smallest value in RST
//         newParentNode->keys[0] = newLeafNode->keys[0];
    
//         // Update the Height of the Tree
//         this->height += 1;

//         // Update the Root of the Tree
//         this->root = newParentNode;

//         // End Method, Return 0 for no issues
//         return 0;
//     }
//     // Parent Node is not Full
//     else if (parentNode->numKeysInserted < NUM_KEYS) {
//         // Update the Parent Node Directly
//         InternalNode *existingParentNode = targetLeafNode->parent;
        
//         // New Key to be inserted will be the largest value since parent keys are already sorted
//         float newKey = newLeafNode->keys[0];
//         // Cannot update the numKeys until we insert the new key
//         existingParentNode->keys[existingParentNode->numKeysInserted] = newKey;
//         existingParentNode->children[existingParentNode->numChildrenNodes] = newLeafNode;

//         // Update the Number of Keys Inserted
//         existingParentNode->numKeysInserted += 1;
//         existingParentNode->numChildrenNodes += 1;

//         // Don;t need update parent, end process
//         return 0;
//     }
//     // Parent Node is Full
//     else if (parentNode->numKeysInserted >= NUM_KEYS) {
//         /**
//          * @brief The keys in the parent nodes are already sorted.
//          * So the new key-child pair will be the largest and to the
//          * right.
//          */

//         // Min Internal Nodes: Floor(n/2) = 19 when n is 39
//         int numKeysLeftParent;
//         int numKeysRightParent;

//         // Determine the Number of Keys each node should have
//         if ((parentNode->numKeysInserted+1) % 2 == 0) {
//             // Even no. of nodes, so split equally
//             numKeysLeftParent = (parentNode->numKeysInserted + 1) / 2;
//             numKeysRightParent = (parentNode->numKeysInserted + 1) / 2;
//         }
//         else {
//             // Odd no. of nodes, left child takes 1 more
//             numKeysLeftParent = std::floor((parentNode->numKeysInserted + 1) / 2) + 1;
//             numKeysRightParent = std::floor((parentNode->numKeysInserted + 1) / 2);
//         }

//         // Create a Sibling Node
//         InternalNode *newSiblingNode = (InternalNode*) malloc(sizeof(InternalNode));

//         // Copy over the Keys and Children of original node to the sibling node
//         int counter = 0;
//         for (int i{numKeysLeftParent}; i < (parentNode->numKeysInserted); i += 1) {
//             // Sibling Node starts from 0, while Parent Node starts from Middle cos larger
//             newSiblingNode->keys[counter] = parentNode->keys[i];
//             newSiblingNode->children[counter] = parentNode->children[i];

//             counter += 1;
//         }
//         // Add the Last Key to the Rightmost Sibling Node {Smallest Key of New Leaf Node}
//         newSiblingNode->keys[counter] = newLeafNode->keys[0];
//         newSiblingNode->children[counter] = newLeafNode;

//         // Have the Last Pointer point to NULL
//         newSiblingNode->children[NUM_KEYS-1] = NULL;

//         // Update Attributes for Sibling Node
//         newSiblingNode->numChildrenNodes = numKeysRightParent;
//         newSiblingNode->numKeysInserted = numKeysRightParent;

//         // Update Attributes for Original parent Node
//         parentNode->numChildrenNodes = numKeysLeftParent;
//         parentNode->numKeysInserted = numKeysLeftParent;

//         // Parent Node to Point to sibling Node
//         parentNode->children[NUM_KEYS-1] = newSiblingNode;

//         // Update for Grandparent Node Recursively
//         InternalNode *grandparentNode = parentNode->parent;
        
//         // Remember Parent is Smaller than its sibling
//         return updateParentNode(grandparentNode, parentNode, newSiblingNode);
//     }

// }

bool BPlusTree :: findRecordInTree(float key, std::stack<Node*> *stackPtr, Record **recordPtr) {
    
    stackPtr->push(root);
    Node* next;
    Record* r = NULL;
    bool found = false;

    for (int i = 0; i < height-1; i++) {
        for (int j = 0; j < stackPtr->top()->numKeysInserted; j++) {
            if (stackPtr->top()->keys[j] >= key) {
                next = ((InternalNode*) stackPtr->top())->children[j];
                break;
            }
        }
        // If next hasn't been assigned, means it refers to the last node pointer
        (next == NULL) && (next = ((InternalNode*) stackPtr->top())->children[stackPtr->top()->numKeysInserted]);
        stackPtr->push(next);
        next = NULL;
    }

    for (int j = 0; j < stackPtr->top()->numKeysInserted; j++) {
        if (stackPtr->top()->keys[j] == key) {
            r = ((LeafNode*) stackPtr->top())->records[j];
            found = true;
            break;
        }
    }

    (found == true) && ((*recordPtr) = r);

    return found;
}