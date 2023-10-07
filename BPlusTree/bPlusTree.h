#ifndef B_PLUS_TREE
#define B_PLUS_TREE
// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stack>
#include <vector>
#include <tuple>
#include "../Record/record.h"
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

        void printRootNodeKeys() {
            // Print out all the Keys of the Roots
            for (int i = 0; i < (this->root->numKeysInserted); i += 1) {
                std::cout << this->root->keys[i] << " ";
            }
        }

        /**
         * Mutator used for test_deletion. To be removed
        */
        void setHeight(int v) {
            height = v;
        }

        /**
         * @brief Set the Root object
         * 
         * @param node 
         */
        void setRoot(Node* node) {
            root = node;
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
         * @brief find all records with key values between key1 and key2, inclusive
         * 
         * @param key1 
         * @param key2 
         * @return the start and end pointers for range query within blockManager 
         */
        std::pair<Record*, Record*> findRecordsInRange(float key1, float key2);

        /**
         * @brief Insert a key into the index
         * 
         * @param key 
         */
        int insertKeyInTree(float key, Record* targetRecord);

        /**
         * @brief Find a node with specified key and return its ancestry
         * 
         * @param key Key to look for
         * 
         * @return Stack containing pairs of parent nodes and key offset
        */
        std::vector<std::pair<Node*, int> > _ancestry(float key);

        /**
         * @brief Recursively update upstream internal nodes
         * 
         * @param st Stack of parent nodes & offsets
        */
        void _updateUpstream(Node*, std::vector<std::pair<Node*, int> > st);

        /**
         * @brief rebalancing of index after a key is deleted from the database
         * 
         * @param deletedKey
         */
        void updateIndex(float deletedKey);
        
        /**
         * @brief Print tree to console. Temp function used for testing. To be removed
        */
        void print();

        /**
         * @brief Determines the Number of Nodes (Internal & Leaf) in a B+ Tree given the Root
         * 
         * @return int 
         */
        int countNodesInBPlusTree(Node* rootNode);
};

#endif