#include "../BPlusTree/bPlusTree.h"
#include "../loadData.h"
#include <cstring>


int test_deletion() {
    BPlusTree* bt = new BPlusTree(); 

    InternalNode* a1 = new InternalNode();

    InternalNode* b1 = new InternalNode();
    InternalNode* b2 = new InternalNode();

    InternalNode* c1 = new InternalNode();
    InternalNode* c2 = new InternalNode();
    InternalNode* c3 = new InternalNode();
    InternalNode* c4 = new InternalNode();

    LeafNode* d1 = new LeafNode();
    LeafNode* d2 = new LeafNode();
    LeafNode* d3 = new LeafNode();
    LeafNode* d4 = new LeafNode();
    LeafNode* d5 = new LeafNode();
    LeafNode* d6 = new LeafNode();
    LeafNode* d7 = new LeafNode();
    LeafNode* d8 = new LeafNode();

    Record* r1 = new Record();
    Record* r2 = new Record();
    Record* r3 = new Record();
    Record* r4 = new Record();
    Record* r5 = new Record();
    Record* r6 = new Record();
    Record* r7 = new Record();
    Record* r8 = new Record();
    Record* r9 = new Record();
    Record* r10 = new Record();
    Record* r11 = new Record();
    Record* r12 = new Record();
    Record* r13 = new Record();
    Record* r14 = new Record();
    Record* r15 = new Record();
    Record* r16 = new Record();

    a1->keys[0] = 9;
    a1->numKeysInserted = 1;
    a1->children[0] = b1;
    a1->children[1] = b2;

    b1->keys[0] = 5;
    b1->numKeysInserted=1;
    b1->children[0] = c1;
    b1->children[1] = c2;

    b2->keys[0] = 13;
    b2->numKeysInserted=1;
    b2->children[0] = c3;
    b2->children[1] = c4;


    c1->keys[0] = 3;
    c1->numKeysInserted=1;
    c1->children[0] = d1;
    c1->children[1] = d2;

    c2->keys[0] = 7;
    c2->numKeysInserted=1;
    c2->children[0] = d3;
    c2->children[1] = d4;

    c3->keys[0] = 11;
    c3->numKeysInserted=1;
    c3->children[0] = d5;
    c3->children[1] = d6;

    c4->keys[0] = 15;
    c4->numKeysInserted=1;
    c4->children[0] = d7;
    c4->children[1] = d8;

    d1->keys[0] = 1;
    d1->keys[1] = 2;
    d1->numKeysInserted = 2;
    d1->records[0] = r1;
    d1->records[1] = r2;

    d2->keys[0] = 3;
    d2->keys[1] = 4;
    d2->numKeysInserted = 2;
    d2->records[0] = r3;
    d2->records[1] = r4;

    d3->keys[0] = 5;
    d3->keys[1] = 6;
    d3->numKeysInserted = 2;
    d3->records[0] = r5;
    d3->records[1] = r6;

    d4->keys[0] = 7;
    d4->keys[1] = 8;
    d4->numKeysInserted = 2;
    d4->records[0] = r7;
    d4->records[1] = r8;

    d5->keys[0] = 9;
    d5->keys[1] = 10;
    d5->numKeysInserted = 2;
    d5->records[0] = r9;
    d5->records[1] = r10;

    d6->keys[0] = 11;
    d6->keys[1] = 12;
    d6->numKeysInserted = 2;
    d6->records[0] = r11;
    d6->records[1] = r12;

    d7->keys[0] = 13;
    d7->keys[1] = 14;
    d7->numKeysInserted = 2;
    d7->records[0] = r13;
    d7->records[1] = r14;

    d8->keys[0] = 15;
    d8->keys[1] = 16;
    d8->numKeysInserted = 2;
    d8->records[0] = r15;
    d8->records[1] = r16;

    r1->fg3PctHomeByteArray = floatToBytes(1.0f);
    r2->fg3PctHomeByteArray = floatToBytes(2.0f);
    r3->fg3PctHomeByteArray = floatToBytes(3.0f);
    r4->fg3PctHomeByteArray = floatToBytes(4.0f);
    r5->fg3PctHomeByteArray = floatToBytes(5.0f);
    r6->fg3PctHomeByteArray = floatToBytes(6.0f);
    r7->fg3PctHomeByteArray = floatToBytes(7.0f);
    r8->fg3PctHomeByteArray = floatToBytes(8.0f);
    r9->fg3PctHomeByteArray = floatToBytes(9.0f);
    r10->fg3PctHomeByteArray = floatToBytes(10.0f);
    r11->fg3PctHomeByteArray = floatToBytes(11.0f);
    r12->fg3PctHomeByteArray = floatToBytes(12.0f);
    r13->fg3PctHomeByteArray = floatToBytes(13.0f);
    r14->fg3PctHomeByteArray = floatToBytes(14.0f);
    r15->fg3PctHomeByteArray = floatToBytes(15.0f);
    r16->fg3PctHomeByteArray = floatToBytes(16.0f);

    bt->setRoot(a1);
    bt->setHeight(4);

    bt->updateIndex(9);
    bt->updateIndex(10);
    bt->updateIndex(11);
    bt->updateIndex(12);
    bt->updateIndex(13);
    bt->updateIndex(14);
    bt->updateIndex(15);
    bt->updateIndex(1);
    bt->updateIndex(2);
    bt->updateIndex(3);
    bt->print();

    free(b1);
    free(b2);
    free(c1);
    free(c2);
    free(c3);
    free(c4);
    free(d1);
    free(d2);
    free(d3);
    free(d4);
    free(d5);
    free(d6);
    free(d7);
    free(d8);
    free(bt);
}


// g++ _x.cpp "BPlusTree/bPlusTree.cpp" "Node/node.cpp" "loadData.cpp" -o _x; .\_x

/*
Test cases


------------ Test 1 ------------
* leaf has sufficient keys
* update parent key

delete 3
9 5 13 4 7 11 15 1 2 4 null 5 6 7 8 9 10 11 12 13 14 15 16

------------ Test 2 ------------
* leaf has sufficient keys
* update grandparent key

delete 5
9 6 13 3 7 11 15 1 2 3 4 6 null 7 8 9 10 11 12 13 14 15 16


------------ Test 3 ------------
* borrow from left sibling

delete 7
9 5 13 3 8 11 15 1 2 3 4 5 6 8 null 9 10 11 12 13 14 15 15

delete 8
9 5 13 3 6 11 15 1 2 3 4 5 null 6 null 9 10 11 12 13 14 15 15

------------ Test 4 ------------
* borrow from right sibling

delete 9
10 5 13 3 7 11 15 1 2 3 4 5 6 7 8 10 null 11 12 13 14 15 16

delete 10
11 5 13 3 7 12 15 1 2 3 4 5 6 7 8 11 null 12 null 13 14 15 16


------------ Test 5 ------------
* recursive delete left

delete 4
delete 3
9 5 13 2 7 11 15 1 null 2 null 5 6 7 8 9 10 11 12 13 14 15 16

delete 2
9 13 5 7 11 null 15 null 1 null 5 6 7 8 9 10 11 12 13 14 15 16


------------ Test 6 ------------
* recursive delete right I

delete 9
delete 10
11 5 13 3 7 11 15 1 2 3 4 5 6 7 8 11 null 12 null 13 14 15 16

delete 11
5 13 3 null 7 null 13 15 1 2 3 4 5 6 7 8 12 null 13 14 15 16


------------ Test 7 ------------
* recursive delete right II

delete 9
delete 10
delete 11
5 12 3 null 7 null 13 15 1 2 3 4 5 6 7 8 12 null 13 14 15 16

delete 12
5 13 3 7 14 15 1 2 3 4 5 6 7 8 13 null 14 null 15 16

delete 13
5 14 3 7 15 1 2 3 4 5 6 7 8 14 null 15 16

delete 14
5 14 3 7 15 1 2 3 4 5 6 7 8 14 null 15 16

delete 15
5 3 null 7 16 1 2 3 4 5 6 7 8 16 null

delete 1
delete 2
delete 3
7 5 16 4 null 5 6 7 8 16 null
*/