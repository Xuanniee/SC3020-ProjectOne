#include <cstring>
#include <cstdlib>
#include "node.h"


// ---------------- InternalNode ----------------
void InternalNode :: mergeLeft(InternalNode* left) {
    int n = (left->numKeysInserted)++;
    Node* curr;

    // copy from right to left
    std::memcpy(left->keys+n, keys, numKeysInserted++*sizeof(float));
    std::memcpy(left->children+n+1, children, numKeysInserted*sizeof(Node*));

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
    Node* curr;

    // make space in dest
    std::memcpy(right->keys+(++numKeysInserted), right->keys, n*sizeof(float));
    std::memcpy(right->children+numKeysInserted, right->children, (n+1)*sizeof(Node*));

    // copy from left to right
    std::memcpy(right->children, children, (numKeysInserted--)*sizeof(Node*));
    std::memcpy(right->keys, keys, numKeysInserted*sizeof(float));

    curr = right->children[numKeysInserted+1];
    while (dynamic_cast<InternalNode*>(curr)) {
        curr = ((InternalNode*) curr)->children[0];
    }
    right->keys[numKeysInserted] = curr->keys[0];
    right->numKeysInserted += numKeysInserted+1;
    free(this);
}