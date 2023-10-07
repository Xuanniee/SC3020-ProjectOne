#include <cstring>
#include <cstdlib>
#include "../BPlusTree/utils.h"
#include "node.h"


// ---------------- InternalNode ----------------
void InternalNode :: mergeLeft(InternalNode* left) {
    int n = (left->numKeysInserted)++;
    Node* curr;

    // copy from right to left
    std::memmove(left->keys+n, keys, numKeysInserted++*sizeof(float));
    std::memmove(left->children+n+1, children, numKeysInserted*sizeof(Node*));

    curr = children[0];
    while (dynamic_cast<InternalNode*>(curr)) {
        curr = ((InternalNode*) curr)->children[0];
    }
    left->keys[numKeysInserted] = curr->keys[0];
    left->numKeysInserted += numKeysInserted-1;
    free(this);
}


void InternalNode :: mergeRight(InternalNode* right) {
    int n = right->numKeysInserted;

    // make space in dest
    std::memmove(right->keys+(++numKeysInserted), right->keys, n*sizeof(float));
    std::memmove(right->children+numKeysInserted, right->children, (n+1)*sizeof(Node*));

    // copy from left to right
    std::memmove(right->children, children, (numKeysInserted--)*sizeof(Node*));
    std::memmove(right->keys, keys, numKeysInserted*sizeof(float));

    right->keys[numKeysInserted] = _leftmost(right->children[numKeysInserted+1]);
    right->numKeysInserted += numKeysInserted+1;
    free(this);
}


// ---------------- LeafNode ----------------
void LeafNode :: mergeLeft(LeafNode* left) {
    int n = left->numKeysInserted;

    // copy from right to left
    std::memmove(left->keys+n, keys, numKeysInserted*sizeof(float));
    std::memmove(left->records+n, records, numKeysInserted*sizeof(Node*));

    left->numKeysInserted += numKeysInserted;
    free(this);
}


void LeafNode :: mergeRight(LeafNode* right) {
    int n = right->numKeysInserted;

    // make space in dest
    std::memmove(right->keys+numKeysInserted, right->keys, n*sizeof(float));
    std::memmove(right->records+numKeysInserted, right->records, n*sizeof(Node*));

    // copy from left to right
    std::memmove(right->records, records, numKeysInserted*sizeof(Node*));
    std::memmove(right->keys, keys, numKeysInserted*sizeof(float));

    right->numKeysInserted += numKeysInserted;
    free(this);
}