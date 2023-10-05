#include "../Block Manager/blockManager.h"


#define NUM_KEYS 39


typedef struct Node {
    // Given the size of the key and the size of a pointer, the number of keys/pointers should be 39
    float keys[NUM_KEYS];
    int numKeysInserted;
};


typedef struct InternalNode: Node {
    Node* children[NUM_KEYS+1];
    int numChildrenNodes;

    // Added Parent Pointer so that I can reference the parent for insertion to update
    struct InternalNode* parent = NULL;

    void merge(InternalNode sibling, InternalNode parent);    
};


typedef struct LeafNode: Node {
    Record* records[NUM_KEYS];
    int numRecordsInserted;
    LeafNode* next;

    // Added Parent Pointer so that I can reference the parent for insertion to update
    InternalNode* parent = NULL;
};