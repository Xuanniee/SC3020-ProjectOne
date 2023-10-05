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

    /**
     * @brief Merge current node with a sibling internal node on the right
     * 
     * @param right Sibling internal node on the right of this node
     * @param parent Parent internal node of this and the sibling
    */
    void merge(InternalNode* right, InternalNode* parent);    
};


typedef struct LeafNode: Node {
    Record* records[NUM_KEYS];
    int numRecordsInserted;
    LeafNode* next;

    // Added Parent Pointer so that I can reference the parent for insertion to update
    InternalNode* parent = NULL;

    /**
     * @brief Merge current node with a sibling leaf node on the right
     * 
     * @param right Sibling leaf node on the right of this node
     * @param parent Parent internal node of this and the sibling
    */
    void merge(LeafNode* right, InternalNode* parent);
};