#include "node.h"


float InternalNode :: merge(InternalNode* right, InternalNode* parent) {
    Node** _children = right->children;
    float* _keys = right->keys;
    
    for (int i=0; i<right->numKeysInserted; i++) {
        keys[numKeysInserted+i] = _keys[i];
        children[numKeysInserted+i+1] = _children[i] ? _children[i] : _children[i+1];
    }

    return keys[0];
}    


float LeafNode :: merge(LeafNode* right, InternalNode* parent) {
    Record** _records = right->records;
    float* _keys = right->keys;
    
    for (int i=0; i<right->numKeysInserted; i++) {
        keys[numKeysInserted+i] = _keys[i];
        records[numKeysInserted+i+1] = _records[i] ? _records[i] : _records[i+1];
    }

    return keys[0];
}