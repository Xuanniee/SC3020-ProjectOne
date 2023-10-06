#ifndef B_PLUS_TREE
#define B_PLUS_TREE
// Import Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stack>
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

        void setRoot(Node* node) {
            root = node;
        }

        /**
         * Mutator used for test_deletion. To be removed
        */
        void setHeight(int v) {
            height = v;
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
        std::vector<std::pair<Node*, int>> _ancestry(float key);

        /**
         * @brief Recusively update upstream internal nodes
         * 
         * @param st Stack of parent nodes & offsets
        */
        void _updateUpstream(Node*, std::vector<std::pair<Node*, int>> st);

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
};

// ------------------- Helper functions -------------------

/**
 * @brief Update the first left parent key with smallest value in current subtree
 * 
 * @param st Ancestry stack
 * @param key Smallest key value
*/
void _updateFirstLeft( std::vector<std::pair<Node*, int>> st, float key);


/**
 * @brief Find a leaf sibling that can have afford to borrow 1 key
 * 
 * @param parent Parent node
 * @param offset Current leaf node's offset within parent's children
 * 
 * @return Offset of sibling if exists, else -1 
*/
int _leafSibling(InternalNode* parent, int offset);


/**
 * @brief Find a sibling that can have afford to borrow 1 key
 * 
 * @param parent Parent node
 * @param offset Current node's offset within parent's children
 * 
 * @return Offset of sibling if exists, else -1 
*/
int _sibling(InternalNode* parent, int offset);


/**
 * @brief Shift keys & pointers
 * 
 * @param node Internal node
 * @param dir Direction
 * @param src Source index

*/
void _shift(InternalNode* node, int src, int dir);


/**
 * @brief Shift keys & pointers
 * 
 * @param node Leaf node 
 * @param src Source index
 * @param dir Direction
*/
void _shift(LeafNode* node, int src, int dir);


#endif