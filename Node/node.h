#ifndef NODE_H
#define NODE_H
#include "../Record/record.h"


#define NUM_KEYS 39
#define MIN_LEAF_KEYS (NUM_KEYS+1)/2
#define MIN_INTERNAL_KEYS NUM_KEYS/2


typedef struct Node {
    // Given the size of the key and the size of a pointer, the number of keys/pointers should be 39
    float keys[NUM_KEYS];
    int numKeysInserted;

    virtual ~Node() {};
} Node;


typedef struct InternalNode: Node {
    Node* children[NUM_KEYS+1];

    /**
     * @brief Merge current node with a sibling internal node on the left
     * 
     * @param left Sibling internal node on the left of this node
    */
    void mergeLeft(InternalNode* left);    


    /**
     * @brief Merge current node with a sibling internal node on the right
     * 
     * @param right Sibling internal node on the right of this node
    */
    void mergeRight(InternalNode* right);    
} InternalNode;


typedef struct LeafNode: Node {
    Record* records[NUM_KEYS];
    LeafNode* next;
} LeafNode;

#endif