#ifndef B_PLUS_TREE
#define B_PLUS_TREE
// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../Block Manager/blockManager.h"

#define NUM_KEYS 39

typedef struct node {
    // Given the size of the key and the size of a pointer, the number of keys/pointers should be 39
    float keys[NUM_KEYS];
} Node;

typedef struct leafNode: Node {
    Record* records[NUM_KEYS];
    leafNode* next;
} LeafNode;

typedef struct internalNode: Node {
    Node* children[NUM_KEYS+1];
} InternalNode;

class BPlusTree {
    private:
        // Need to be cast to either InternalNode* or LeafNode* accordingly
        Node* root;

        // If height == 1, then root is a LeafNode* pointer
        int height;

    public:
        /**
         * @brief Get the Height
         * 
         * @return int 
         */
        int getHeight() {
            return height;
        }

        /**
         * @brief Get the Root
         * 
         * @return Node* 
         */
        Node* getRoot() {
            return root;
        }

        /**
         * @brief Search the tree for a record given the record key
         * If there are duplicate records, only the address of the first record with the key is returned
         * 
         * @param key 
         * @return Record* 
         */
        Record* findRecordInTree(float key);

        /**
         * @brief rebalancing of index after a key is deleted from the database
         * 
         * @param deletedKey 
         */
        void updateIndex(float deletedKey);

        /**
         * @brief Insert a key into the index
         * 
         * @param key 
         */
        void insertKey(float key);
};


#endif