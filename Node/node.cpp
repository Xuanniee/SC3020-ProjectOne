#include <string.h>
#include "node.h"
#include "../loadData.h"


// ---------------- InternalNode ----------------
void InternalNode :: merge(InternalNode* right) {
    Node** _children = right->children;
    float* _keys = right->keys;
    int i;
    
    for (i=0; i<right->numKeysInserted; i++) {
        keys[numKeysInserted+i] = _keys[i];
        children[numKeysInserted+i+1] = _children[i];
    }
    children[numKeysInserted+i+1] = _children[i];
}    


void InternalNode :: borrowLeft(InternalNode* left) {
    // shift all data down by 1 slot
    std::memcpy(children+1, children, 1);
    std::memcpy(keys, keys+1, 1);

    // copy over borrowed data from left sibling
    children[0] = left->children[left->numKeysInserted];
    (left->numKeysInserted)--;
    keys[0] = children[1]->keys[0];
}


void InternalNode :: borrowRight(InternalNode* right) {
    // copy over borrowed data from right sibling
    keys[numKeysInserted] = right->children[0]->keys[0];
    numKeysInserted++;
    children[numKeysInserted] = right->children[0];
    
    // shift all data down by 1 slot (in sibling)
    std::memcpy(right->children, right->children+1, right->numKeysInserted);
    (right->numKeysInserted)--;
    std::memcpy(right->keys, right->keys+1, right->numKeysInserted);
}


// ---------------- LeafNode ----------------
void LeafNode :: merge(LeafNode* right) {
    Record** _records = right->records;
    float* _keys = right->keys;
    int i;
    
    for (i=0; i<right->numKeysInserted; i++) {
        keys[numKeysInserted+i] = _keys[i];
        records[numKeysInserted+i+1] = _records[i];
    }
    records[numKeysInserted+i+1] = _records[i];
}


void LeafNode :: borrowLeft(LeafNode* left) {
    // shift all data down by 1 slot
    std::memcpy(records+1, records, numKeysInserted+1);
    std::memcpy(keys+1, keys, numKeysInserted);

    // copy over borrowed data from left sibling
    (left->numKeysInserted)--;
    records[0] = left->records[left->numKeysInserted];
    keys[0] = left->keys[left->numKeysInserted];
}


void LeafNode :: borrowRight(LeafNode* right) {
    // copy over borrowed data from right sibling
    records[numKeysInserted] = right->records[0];
    keys[numKeysInserted] = right->keys[0];
    numKeysInserted++;
    
    // shift all data down by 1 slot (in sibling)
    right->numKeysInserted--;
    std::memcpy(right->records, right->records+1, right->numKeysInserted);
    std::memcpy(right->keys, right->keys+1, right->numKeysInserted);
}