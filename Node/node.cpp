#include <cstring>
#include "node.h"


// ---------------- InternalNode ----------------
void InternalNode :: mergeLeft(InternalNode* left) {
    int n = (left->numKeysInserted)++;
    Node* curr;

    std::cout << "[mergeLeft] n: " << n << std::endl;
    std::cout << "[mergeLeft] numKeysInserted: " << numKeysInserted << std::endl;

    std::memcpy(left->keys+n, keys, numKeysInserted++*sizeof(float));
    std::memcpy(left->children+n+1, children, numKeysInserted*sizeof(Node*));
    std::cout << "[mergeLeft] left->keys: " << left->keys[0] << "," << left->keys[1] << std::endl;

    curr = children[0];
    while (dynamic_cast<InternalNode*>(curr)) {
        curr = ((InternalNode*) curr)->children[0];
    }
    left->keys[numKeysInserted] = curr->keys[0];

    std::cout << "[mergeLeft] left->keys: " << left->keys[0] << "," << left->keys[1] << std::endl;
    std::cout << "[mergeLeft] left->numKeys: " << left->numKeysInserted << std::endl;
    left->numKeysInserted += numKeysInserted-1;

    free(this);
}


void InternalNode :: mergeRight(InternalNode* right) {
    int n = right->numKeysInserted;
    Node* curr;

    std::cout << "[mergeRight] right->keys[0]: " << right->keys[0] << ", n: " << n << ", numKeys: " << numKeysInserted << std::endl;

    // make space in dest
    std::memcpy(right->keys+(++numKeysInserted), right->keys, n*sizeof(float));
    std::memcpy(right->children+numKeysInserted, right->children, (n+1)*sizeof(Node*));

    std::cout << "[mergeRight] right->keys: " << right->keys[0] << "," << right->keys[1] << std::endl;

    std::cout << "XXX: " << children[0]->keys[0] << std::endl;
    // copy from left to right
    std::memcpy(right->children, children, (numKeysInserted--)*sizeof(Node*));
    std::memcpy(right->keys, keys, numKeysInserted*sizeof(float));
    std::cout << "[mergeRight] numKeys: " << numKeysInserted << std::endl;
    std::cout << "[mergeRight] right->keys: " << right->keys[0] << "," << right->keys[1] << std::endl;
    
    curr = right->children[numKeysInserted+1];
    while (dynamic_cast<InternalNode*>(curr)) {
        curr = ((InternalNode*) curr)->children[0];
    }
    right->keys[numKeysInserted] = curr->keys[0];

    std::cout << "[mergeRight] right->keys: " << right->keys[0] << "," << right->keys[1] << std::endl;
    right->numKeysInserted += numKeysInserted+1;
    std::cout << "[mergeRight] right->numKeys: " << right->numKeysInserted << std::endl;
    free(this);
}