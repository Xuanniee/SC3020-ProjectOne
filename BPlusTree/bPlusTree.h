#ifndef B_PLUS_TREE
#define B_PLUS_TREE
// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stack>
#include "../Block Manager/blockManager.h"
#include "../Node/node.h"


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
         * @param key -- search key
         * @param stackPtr -- the pointer to the stack
         * @param recordPtr -- pointer to the pointer to a record
         * @return bool -- whether the record has been found
         */
        bool findRecordInTree(float key, std::stack<Node*> *stackPtr, Record **recordPtr);

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
        int insertKeyInTree(float key, Record* targetRecord);
};


#endif