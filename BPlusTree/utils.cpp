#include <cstring>
#include <tuple>
#include <vector>
#include "bPlusTree.h"
#include "utils.h"


std::vector<std::pair<Node*, int> > BPlusTree :: _ancestry(float key) {

    std::vector<std::pair<Node*, int> > res;
    Node* curr = root;
    int _, i;
    float *keys;

    // internal nodes
    for (_=1; _<height; _++) {
        keys = curr->keys;
        for (i=0; i<curr->numKeysInserted; i++) {
            if (keys[i] > key) break;
        }
        res.push_back(std::make_pair(curr, i));
        curr = ((InternalNode*) curr)->children[i];
    }

    // leaf node
    keys = curr->keys;
    for (i=0; i<curr->numKeysInserted; i++) {
        if (keys[i] == key) {
            res.push_back(std::make_pair(curr, i));
            return res;
        }
    }
    return std::vector<std::pair<Node*, int> >();
}


void _shift(LeafNode* node, int src, int dir) {
    int n = (dir==1 ? node->numKeysInserted++ : node->numKeysInserted--) - src;
    if (!n || src+dir<0) return;
    std::memcpy(node->records+src+dir, node->records+src, n*sizeof(Record*));
    std::memcpy(node->keys+src+dir, node->keys+src, n*sizeof(float)); 
}


void _shift(InternalNode* node, int src, int dir) {
    int n = (dir==1 ? node->numKeysInserted++ : node->numKeysInserted--) - src;
    if (n>=0) std::memcpy(node->children+src+dir, node->children+src, (n+1)*sizeof(Record*));
    if (n>0) std::memcpy(node->keys+src+dir, node->keys+src, n*sizeof(float)); 
}


int _sibling(InternalNode* parent, int offset) {

    if (offset > 0 && parent->children[offset-1]->numKeysInserted > MIN_INTERNAL_KEYS) return offset-1;
    if (offset < parent->numKeysInserted && parent->children[offset+1]->numKeysInserted > MIN_INTERNAL_KEYS) return offset+1;
    return -1;
}


int _leafSibling(InternalNode* parent, int offset) {

    if (offset > 0 && parent->children[offset-1]->numKeysInserted > MIN_LEAF_KEYS) return offset-1;
    if (offset < parent->numKeysInserted && parent->children[offset+1]->numKeysInserted > MIN_LEAF_KEYS) return offset+1;
    return -1;
}


void _updateFirstLeft( std::vector<std::pair<Node*, int> > st, float key) {
    // update first left parent's key
    for (int i=st.size()-1; i>=0; i--) {
        if (st[i].second > 0) {
            ((InternalNode*) st[i].first)->keys[st[i].second-1] = key;
            break;
        }
    }
}


float _leftmost(Node* node) {
    while (dynamic_cast<InternalNode*>(node)) {
        node = ((InternalNode*) node)->children[0];
    }
    return node->keys[0];
}