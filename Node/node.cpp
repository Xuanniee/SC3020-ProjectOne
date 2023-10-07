#include <cstring>
#include <cstdlib>
#include "../BPlusTree/utils.h"
#include "node.h"


// ---------------- InternalNode ----------------
void InternalNode :: mergeLeft(InternalNode* left) {
    int n = (left->numKeysInserted)++;

    // copy from right to left
    std::memmove(left->keys+n, keys, numKeysInserted++*sizeof(float));
    std::memmove(left->children+n+1, children, numKeysInserted*sizeof(Node*));

    left->keys[numKeysInserted] = _leftmost(children[0]);
    left->numKeysInserted += numKeysInserted-1;
    free(this);
}


void InternalNode :: mergeRight(InternalNode* right) {
    int n = right->numKeysInserted;

    // make space in dest
    std::memmove(right->keys+(++numKeysInserted), right->keys, n*sizeof(unsigned short int));
    std::memmove(right->children+numKeysInserted, right->children, (n+1)*sizeof(Node*));

    // copy from left to right
    std::memmove(right->children, children, (numKeysInserted--)*sizeof(Node*));
    std::memmove(right->keys, keys, numKeysInserted*sizeof(unsigned short int));

    right->keys[numKeysInserted] = _leftmost(right->children[numKeysInserted+1]);
    right->numKeysInserted += numKeysInserted+1;
    free(this);
}


// ---------------- LeafNode ----------------
void LeafNode :: mergeLeft(LeafNode* left) {
    int n = left->numKeysInserted;

    // copy from right to left
    std::memmove(left->keys+n, keys, numKeysInserted*sizeof(unsigned short int));
    std::memmove(left->records+n, records, numKeysInserted*sizeof(Record*));

    left->numKeysInserted += numKeysInserted;
    free(this);
}


void LeafNode :: mergeRight(LeafNode* right) {
    int n = right->numKeysInserted;

    // make space in dest
    std::memmove(right->keys+numKeysInserted, right->keys, n*sizeof(unsigned short int));
    std::memmove(right->records+numKeysInserted, right->records, n*sizeof(Record*));

    // copy from left to right
    std::memmove(right->records, records, numKeysInserted*sizeof(Record*));
    std::memmove(right->keys, keys, numKeysInserted*sizeof(unsigned short int));

    right->numKeysInserted += numKeysInserted;
    free(this);
}