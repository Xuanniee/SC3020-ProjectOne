// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <stack>
#include <queue>
#include <vector>
#include <tuple>
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
    for (int i = 0; i < numKeysInTarget; i++) {
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
    for (int i = 0; i < numKeysInTarget; i++) {
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

    // Prepare the Variables for the findRecordInTree() function
    std::stack<Node*> myStack;
    std::stack<Node*> *myStackPtr = &myStack;
    // Will retrieve the actual record, but won't be relevant in this function
    Record **recordPtr;

    // Search the Tree to see if the B+ tree contains the Record Key
    bool searchResult = findRecordInTree(key, myStackPtr, recordPtr);

    // Check if the Key Value exists in the B+ Tree
    if (searchResult) {
        // Record exists, do nothing
        return 0;
    }

    // Retrieve the Target Leaf Node, i.e. the node where the target exist or where target is supposed to be inserted
    LeafNode *targetLeafNode = (LeafNode*) myStackPtr->top();
    myStackPtr->pop();
    
    // If the key does not exists in tree, check if there is space
    int numKeysInTarget = targetLeafNode->numKeysInserted;
    // int numRecordsInTarget = targetLeafNode->numRecordsInserted;

    // Case 1 - Leaf Node has space to directly insert
    if (numKeysInTarget < NUM_KEYS) {
        // Insert the Record and Key
        insertionSortInsertLeafNode(key, targetRecord, numKeysInTarget, targetLeafNode);

        // Update the numkeys and numRecords
        targetLeafNode->numKeysInserted += 1;
        // targetLeafNode->numRecordsInserted += 1;
    }

    // Case 2 - Leaf Node is full, has (n+1) keys now. 
    // Create a new sibling leaf node
    LeafNode *newLeafNode = (LeafNode*) malloc(sizeof(LeafNode));
    newLeafNode->next = NULL;

    // Distribute the keys - MUST ALWAYS BE 40 else, sth is wrong
    if ((numKeysInTarget + 1) % 2 == 0) {
        // Even number of keys. Create a New Array to store all the (n + 1) keys since the class array cannot store
        float combinedKeysArray[NUM_KEYS + 1];
        Record *combinedRecordsArray[NUM_KEYS + 1];
        for (int i = 0; i < numKeysInTarget; i++) {
            combinedKeysArray[i] = targetLeafNode->keys[i];
            combinedRecordsArray[i] = targetLeafNode->records[i];
        }

        // Insert the n+1th key and sort all (n+1) keys. Okay to pass by value since don't need the arrays afterwards
        std::pair<float*, Record**> insertionResult = insertionSortInsertArray(key, targetRecord, numKeysInTarget, combinedKeysArray, combinedRecordsArray);
        float *insertionKeysArray = insertionResult.first;
        Record **insertionRecordsArray = insertionResult.second;
        
        // Determine the index to start copying for the larger half for the newly created node
        int numKeysToCopy = numKeysInTarget / 2;

        // Update the Old Leaf Node, i.e. original leaf
        for (int i = 0; i < numKeysToCopy; i++) {
            // Copy the Keys over
            targetLeafNode->keys[i] = *(insertionKeysArray + i);

            // Copy the Record over
            targetLeafNode->records[i] = *(insertionRecordsArray + i);
        }

        // Update the New Leaf Node
        int counter = 0;
        for (int i = numKeysToCopy; i < numKeysInTarget; i++) {
            // New leaf should have no records, so start from 0
            newLeafNode->keys[counter] = *(insertionKeysArray + i);
            newLeafNode->records[counter] = *(insertionRecordsArray + i);

            counter += 1;
        }

        // Update the number of keys inserted in the nodes, split evenly since 39 + 1 == 40
        targetLeafNode->numKeysInserted = numKeysToCopy;
        // targetLeafNode->numRecordsInserted = numKeysToCopy;
        newLeafNode->numKeysInserted = numKeysToCopy;
        // newLeafNode->numRecordsInserted = numKeysToCopy;

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

    // Check if the Parent Node exists
    InternalNode *parentNode; // = targetLeafNode->parent;
    if (myStackPtr->empty()) {
        // No Parent Node, is 1 level B+ Tree
        parentNode = NULL;
    }
    else {
        // Parent Exists, so must be Internal Node
        parentNode = (InternalNode*) myStackPtr->top();
        myStackPtr->pop();
    }

    // Boolean to determine if we are at Level 1.
    bool reachRoot = false;
    do {
        // Case 2.1 - If Parent Does not exist, i.e. currently height is 1
        if (parentNode == NULL) {
            // Create a New Parent Node
            InternalNode *newParentNode = (InternalNode*) malloc(sizeof(InternalNode));
            newParentNode->numChildrenNodes = 2;
            newParentNode->numKeysInserted = 1;
            // newParentNode->parent = NULL;

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
        // Case 2.2 - Parent Node is not Full
        else if (parentNode->numKeysInserted < NUM_KEYS) {
            // Case 2.2.1 - Update the Parent Node Directly
            // InternalNode *existingParentNode = targetLeafNode->parent;
            
            // TBH Not very sure of this
            // New Key to be inserted is the largest value since parent keys are already sorted
            float newKey = newLeafNode->keys[0];

            // Cannot update the numKeys until we insert the new key
            parentNode->keys[parentNode->numKeysInserted] = newKey;
            parentNode->children[parentNode->numChildrenNodes] = newLeafNode;

            // Update the Number of Keys Inserted
            parentNode->numKeysInserted += 1;
            parentNode->numChildrenNodes += 1;

            // Don't need update parent, end process
            reachRoot = true;
        }
        // Case 2.3 - Parent Node is Full
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
            if ((parentNode->numKeysInserted + 1) % 2 == 0) {
                // Even no. of nodes, so split equally
                numKeysLeftParent = (parentNode->numKeysInserted + 1) / 2;
                numKeysRightParent = (parentNode->numKeysInserted + 1) / 2;
            }
            else {
                // Odd no. of nodes, left child takes 1 more
                numKeysLeftParent = std::floor((parentNode->numKeysInserted + 1) / 2) + 1;
                numKeysRightParent = std::floor((parentNode->numKeysInserted + 1) / 2);
            }

            // Create a Sibling Node to Parent, i.e. Uncle Node
            InternalNode *newUncleNode = (InternalNode*) malloc(sizeof(InternalNode));

            // Copy over the Keys and Children of original node to the uncle node
            int counter = 0;
            for (int i = numKeysLeftParent; i < (parentNode->numKeysInserted); i++) {
                // Sibling Node starts from 0, while Parent Node starts from Middle cos larger
                newUncleNode->keys[counter] = parentNode->keys[i];
                newUncleNode->children[counter] = parentNode->children[i];

                counter += 1;
            }
            // Add the Last Key to the Rightmost Sibling Node {Smallest Key of New Leaf Node}
            newUncleNode->keys[counter] = newLeafNode->keys[0];
            newUncleNode->children[counter] = newLeafNode;

            // Have the Last Pointer point to NULL
            newUncleNode->children[NUM_KEYS - 1] = NULL;

            // Update Attributes for Uncle Node (On the Right)
            newUncleNode->numChildrenNodes = numKeysRightParent;
            newUncleNode->numKeysInserted = numKeysRightParent;

            // Update Attributes for Original Parent Node (On the Left)
            parentNode->numChildrenNodes = numKeysLeftParent;
            parentNode->numKeysInserted = numKeysLeftParent;

            // Parent Node to Point to sibling Node
            parentNode->children[NUM_KEYS - 1] = newUncleNode;

            // Update the Parent Node to be the Grandparent Node now
            if (myStackPtr->empty()) {
                // Sets up for Case 2.1
                parentNode = NULL;
            }
            else {
                // Grandparent must also be internal node
                parentNode = (InternalNode*) myStackPtr->top();
                myStackPtr->pop();
            }
            // parentNode = parentNode->parent;
            // Remember Parent is Smaller than its sibling
            // reachRoot still false
            reachRoot = false;
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


std::vector<std::pair<Node*, int> > BPlusTree :: _ancestry(float key) {

    std::vector<std::pair<Node*, int> > res;
    Node* curr = root;
    int _, i;
    float *keys;

    // internal nodes
    for (_=1; _<height; _++) {
        keys = curr->keys;
        for (i=0; i<curr->numKeysInserted; i++) {
            if (keys[i] > key) break;
        }
        res.push_back(std::make_pair(curr, i));
        curr = ((InternalNode*) curr)->children[i];
    }

    // leaf node
    keys = curr->keys;
    for (i=0; i<curr->numKeysInserted; i++) {
        if (keys[i] == key) {
            res.push_back(std::make_pair(curr, i));
            return res;
        }
    }
    return std::vector<std::pair<Node*, int> >();
}


void _shift(LeafNode* node, int src, int dir) {
    int n = (dir==1 ? node->numKeysInserted++ : node->numKeysInserted--) - src;
    if (!n || src+dir<0) return;
    std::memcpy(node->records+src+dir, node->records+src, n*sizeof(Record*));
    std::memcpy(node->keys+src+dir, node->keys+src, n*sizeof(float)); 
}


void _shift(InternalNode* node, int src, int dir) {
    int n = (dir==1 ? node->numKeysInserted++ : node->numKeysInserted--) - src;
    if (n>=0) std::memcpy(node->children+src+dir, node->children+src, (n+1)*sizeof(Record*));
    if (n>0) std::memcpy(node->keys+src+dir, node->keys+src, n*sizeof(float)); 
}


int _sibling(InternalNode* parent, int offset) {

    if (offset > 0 && parent->children[offset-1]->numKeysInserted > MIN_INTERNAL_KEYS) return offset-1;
    if (offset < parent->numKeysInserted && parent->children[offset+1]->numKeysInserted > MIN_INTERNAL_KEYS) return offset+1;
    return -1;
}


int _leafSibling(InternalNode* parent, int offset) {

    if (offset > 0 && parent->children[offset-1]->numKeysInserted > MIN_LEAF_KEYS) return offset-1;
    if (offset < parent->numKeysInserted && parent->children[offset+1]->numKeysInserted > MIN_LEAF_KEYS) return offset+1;
    return -1;
}


void _updateFirstLeft( std::vector<std::pair<Node*, int> > st, float key) {
    // update first left parent's key
    for (int i=st.size()-1; i>=0; i--) {
        if (st[i].second > 0) {
            ((InternalNode*) st[i].first)->keys[st[i].second-1] = key;
            break;
        }
    }
}


void BPlusTree :: _updateUpstream(Node*, std::vector<std::pair<Node*, int> > st) {
    Node* temp;
    InternalNode* node;
    InternalNode* parent;
    InternalNode* sibling;
    int offset, i_sibling, offset_parent;
    
    std::tie(temp, offset) = st.back();
    st.pop_back();
    node = (InternalNode*) temp;

    _shift(node, offset+1, -1); // delete

    if (node == root) {
        if (node->numKeysInserted == 0) {
            root = node->children[0];
            height--;
        } else if (node->numKeysInserted == -1) {
            root = nullptr;
            height--;
        }
        return;
    }
    
    if (node->numKeysInserted >= MIN_INTERNAL_KEYS) return _updateFirstLeft(st, node->children[0]->keys[0]);

    // borrow
    std::tie(temp, offset_parent) = st.back();
    parent = (InternalNode*) temp;
    i_sibling = _sibling(parent, offset);

    if (i_sibling != -1) {
        sibling = (InternalNode*) parent->children[i_sibling];

        if (offset_parent < offset) {
            // borrow from left sibling
            _shift(node, 0, 1); // make space for borrowed key
            node->children[0] = sibling->children[sibling->numKeysInserted--];
            node->keys[0] = sibling->keys[sibling->numKeysInserted];
            parent->keys[offset-1] = node->keys[0];
            return;
        } else {
            // borrow from right sibling
            node->keys[node->numKeysInserted++] = sibling->children[0]->keys[0];
            node->children[node->numKeysInserted] = sibling->children[0];
            _shift(sibling, 1, -1); // delete sibling's first key & ptr
            temp = sibling->children[0];
            while (dynamic_cast<InternalNode*>(temp)) {
                temp = ((InternalNode*) temp)->children[0];
            }
            parent->keys[i_sibling-1] = temp->keys[0];
            return;
        }
    }
    
    if (offset_parent == 0) {
        // merge onto right sibling
        node->mergeRight((InternalNode*) parent->children[offset_parent+1]);
        _updateUpstream(parent, st);
    } else {
        // merge onto left sibling
        node->mergeLeft((InternalNode*) parent->children[offset_parent-1]);
        _updateUpstream(parent, st);
    }
}


void BPlusTree :: updateIndex(float deletedKey) {

    std::vector<std::pair<Node*, int> > st = _ancestry(deletedKey);

    if (st.empty()) return;

    Node* node;
    Node* temp;
    LeafNode* sibling;
    LeafNode* leaf;
    InternalNode* parent;
    int offset, i_sibling, i;
    
    std::tie(node, offset) = st.back();
    leaf = (LeafNode*) node;
    st.pop_back();

    _shift(leaf, offset+1, -1); // delete

    // ------------ CASE 1 ------------
    // Sufficient keys remaining
    
    if (leaf->numKeysInserted >= MIN_LEAF_KEYS) return _updateFirstLeft(st, leaf->keys[0]);
    
    // ------------ CASE 2 ------------
    // Borrow from sibling
    std::tie(temp, offset) = st.back();

    parent = (InternalNode*) temp;
    i_sibling = _leafSibling(parent, offset);

    if (i_sibling != -1) {
        sibling = (LeafNode*) (parent->children[i_sibling]);

        if (node->keys[0] > sibling->keys[0]) {
            // borrow from left sibling
            _shift(leaf, 0, 1); // make space for borrowed key
            leaf->records[0] = sibling->records[--sibling->numKeysInserted];
            leaf->keys[0] = sibling->keys[sibling->numKeysInserted];
            parent->keys[offset-1] = leaf->keys[0];
        } else {
            // borrow from right sibling
            leaf->keys[leaf->numKeysInserted++] = sibling->keys[0];
            leaf->records[leaf->numKeysInserted] = sibling->records[0];
            _shift(sibling, 1, -1); // delete sibling's first key & ptr
            parent->keys[i_sibling-1] = sibling->keys[0];
            _updateFirstLeft(st, leaf->keys[0]);
        }
        return;
    } 

    // ------------ CASE 3 ------------
    // Unable to borrow, hence need to delete itself & parent's ptr
    // since we delete a leaf node, we need to update it's left leaf's pointer

    for (i=st.size()-1; i>=0; i--) {
        offset = st[i].second;
        if (offset > 0) break; 
    }

    if (i > 0) {
        temp = ((InternalNode*) st[i].first)->children[offset-1];
        while (dynamic_cast<InternalNode*>(temp)) {
            temp = ((InternalNode*) temp)->children[temp->numKeysInserted+1];
        }
        ((LeafNode*) temp)->next = leaf->next;
    }
    _updateUpstream(parent, st);
}


void BPlusTree :: print() {
    std::queue<std::queue<Node*> > curr;
    std::queue<std::queue<Node*> > next;
    std::queue<Node*> temp;
    std::queue<Node*> temp2;
    float* keys;
    Node** children;
    Node* child;
    int i, j;

    temp.push(root);
    curr.push(temp);
    for (int _=1; _<height; _++) {
        while (!curr.empty()) {
            temp = curr.front();
            curr.pop();

            std::cout << "[ ";
            
            while (!temp.empty()) {
                temp2 = std::queue<Node*>();
                child = temp.front();
                temp.pop();
                keys = child->keys;
                children = ((InternalNode*) child)->children;
                std::cout << "(";

                for (i = 0; i<child->numKeysInserted; i++) {
                    std::cout << keys[i] << ",";
                    temp2.push(children[i]);
                }
                temp2.push(children[i]);
                next.push(temp2);
                std::cout << ") ";
            }
            std::cout << "] ";
            
        }
        std::cout << std::endl;
        std::swap(curr, next);
        next = std::queue<std::queue<Node*> >();
    }
    while (!curr.empty()) {
        temp = curr.front();
        curr.pop();

        std::cout << "[ ";
        while (!temp.empty()) {
            child = temp.front();
            keys = child->keys;
            temp.pop();

            std::cout << "(";
            for (i=0; i<child->numKeysInserted; i++) {
                std::cout << keys[i] << ",";
            }
            std::cout << ") ";
        }
        std::cout << "] ";
    }
    std::cout<<std::endl;
}