#include <cstring>
#include "../BPlusTree/bPlusTree.h"
#include "../loadData.h"
#include "../Node/node.h"
#include "../Record/record.h"
#include "../Block Manager/blockManager.h"


void test_del_1() {
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

    a1->keys[0] = floatToBytes(9);
    a1->numKeysInserted = 1;
    a1->children[0] = b1;
    a1->children[1] = b2;

    b1->keys[0] = floatToBytes(5);
    b1->numKeysInserted=1;
    b1->children[0] = c1;
    b1->children[1] = c2;

    b2->keys[0] = floatToBytes(13);
    b2->numKeysInserted=1;
    b2->children[0] = c3;
    b2->children[1] = c4;

    c1->keys[0] = floatToBytes(3);
    c1->numKeysInserted=1;
    c1->children[0] = d1;
    c1->children[1] = d2;

    c2->keys[0] = floatToBytes(7);
    c2->numKeysInserted=1;
    c2->children[0] = d3;
    c2->children[1] = d4;

    c3->keys[0] = floatToBytes(11);
    c3->numKeysInserted=1;
    c3->children[0] = d5;
    c3->children[1] = d6;

    c4->keys[0] = floatToBytes(15);
    c4->numKeysInserted=1;
    c4->children[0] = d7;
    c4->children[1] = d8;

    d1->keys[0] = floatToBytes(1);
    d1->keys[1] = floatToBytes(2);
    d1->numKeysInserted = 2;
    d1->records[0] = r1;
    d1->records[1] = r2;

    d2->keys[0] = floatToBytes(3);
    d2->keys[1] = floatToBytes(4);
    d2->numKeysInserted = 2;
    d2->records[0] = r3;
    d2->records[1] = r4;

    d3->keys[0] = floatToBytes(5);
    d3->keys[1] = floatToBytes(6);
    d3->numKeysInserted = 2;
    d3->records[0] = r5;
    d3->records[1] = r6;

    d4->keys[0] = floatToBytes(7);
    d4->keys[1] = floatToBytes(8);
    d4->numKeysInserted = 2;
    d4->records[0] = r7;
    d4->records[1] = r8;

    d5->keys[0] = floatToBytes(9);
    d5->keys[1] = floatToBytes(10);
    d5->numKeysInserted = 2;
    d5->records[0] = r9;
    d5->records[1] = r10;

    d6->keys[0] = floatToBytes(11);
    d6->keys[1] = floatToBytes(12);
    d6->numKeysInserted = 2;
    d6->records[0] = r11;
    d6->records[1] = r12;

    d7->keys[0] = floatToBytes(13);
    d7->keys[1] = floatToBytes(14);
    d7->numKeysInserted = 2;
    d7->records[0] = r13;
    d7->records[1] = r14;

    d8->keys[0] = floatToBytes(15);
    d8->keys[1] = floatToBytes(16);
    d8->numKeysInserted = 2;
    d8->records[0] = r15;
    d8->records[1] = r16;

    d1->next = d2;
    d2->next = d3;
    d3->next = d4;
    d4->next = d5;
    d5->next = d6;
    d6->next = d7;
    d7->next = d8;

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
    r16->fg3PctHomeByteArray = floatToBytes(16.0);

    bt->setRoot(a1);
    bt->setHeight(4);

    BlockManager* bm = new BlockManager();

    bm->deleteRange(bt, 0.0f,3.0f);
    bm->deleteRange(bt, 9.0f, 13.0f);
    bt->print();

    free(a1);
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
    free(r1);
    free(r2);
    free(r3);
    free(r4);
    free(r5);
    free(r6);
    free(r7);
    free(r8);
    free(r9);
    free(r10);
    free(r11);
    free(r12);
    free(r13);
    free(r14);
    free(r15);
    free(r16);
}


void test_del_2() {
    BPlusTree* bt = new BPlusTree(); 

    InternalNode* a1 = new InternalNode();

    InternalNode* b1 = new InternalNode();
    InternalNode* b2 = new InternalNode();
    InternalNode* b3 = new InternalNode();

    LeafNode* c1 = new LeafNode();
    LeafNode* c2 = new LeafNode();
    LeafNode* c3 = new LeafNode();
    LeafNode* c4 = new LeafNode();
    LeafNode* c5 = new LeafNode();
    LeafNode* c6 = new LeafNode();

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

    a1->keys[0] = floatToBytes(6);
    a1->keys[1] = floatToBytes(10);
    a1->numKeysInserted = 2;
    a1->children[0] = b1;
    a1->children[1] = b2;
    a1->children[2] = b3;

    b1->keys[0] = floatToBytes(4);
    b1->numKeysInserted=1;
    b1->children[0] = c1;
    b1->children[1] = c2;

    b2->keys[0] = floatToBytes(8);
    b2->numKeysInserted=1;
    b2->children[0] = c3;
    b2->children[1] = c4;

    b3->keys[0] = floatToBytes(12);
    b3->numKeysInserted=1;
    b3->children[0] = c5;
    b3->children[1] = c6;

    c1->keys[0] = floatToBytes(1);
    c1->keys[1] = floatToBytes(2);
    c1->numKeysInserted=2;
    c1->records[0] = r1;
    c1->records[1] = r2;

    c2->keys[0] = floatToBytes(4);
    c2->keys[1] = floatToBytes(5);
    c2->numKeysInserted=2;
    c2->records[0] = r3;
    c2->records[1] = r4;

    c3->keys[0] = floatToBytes(6);
    c3->keys[1] = floatToBytes(7);
    c3->numKeysInserted=2;
    c3->records[0] = r5;
    c3->records[1] = r6;

    c4->keys[0] = floatToBytes(8);
    c4->keys[1] = floatToBytes(9);
    c4->numKeysInserted=2;
    c4->records[0] = r7;
    c4->records[1] = r8;

    c5->keys[0] = floatToBytes(10);
    c5->keys[1] = floatToBytes(11);
    c5->numKeysInserted=2;
    c5->records[0] = r9;
    c5->records[1] = r10;

    c6->keys[0] = floatToBytes(12);
    c6->keys[1] = floatToBytes(13);
    c6->numKeysInserted=2;
    c6->records[0] = r11;
    c6->records[1] = r12;

    r1->fg3PctHomeByteArray = floatToBytes(1.0f);
    r2->fg3PctHomeByteArray = floatToBytes(2.0f);
    r3->fg3PctHomeByteArray = floatToBytes(4.0f);
    r4->fg3PctHomeByteArray = floatToBytes(5.0f);
    r5->fg3PctHomeByteArray = floatToBytes(6.0f);
    r6->fg3PctHomeByteArray = floatToBytes(7.0f);
    r7->fg3PctHomeByteArray = floatToBytes(8.0f);
    r8->fg3PctHomeByteArray = floatToBytes(9.0f);
    r9->fg3PctHomeByteArray = floatToBytes(10.0f);
    r10->fg3PctHomeByteArray = floatToBytes(11.0f);
    r11->fg3PctHomeByteArray = floatToBytes(12.0f);
    r12->fg3PctHomeByteArray = floatToBytes(13.0f);

    bt->setRoot(a1);
    bt->setHeight(3);
    bt->updateIndex(floatToBytes(4));
    bt->updateIndex(floatToBytes(13));
    bt->print();

    free(a1);
    free(b1);
    free(b2);
    free(b3);
    free(c1);
    free(c2);
    free(c3);
    free(c4);
    free(c5);
    free(c6);
    free(bt);
    free(r1);
    free(r2);
    free(r3);
    free(r4);
    free(r5);
    free(r6);
    free(r7);
    free(r8);
    free(r9);
    free(r10);
    free(r11);
    free(r12);
}


void test_deletion() {
    /**
     * ----- Test 1 -----
     * 
     * Set Node/node.h NUM_KEYS 2
     * 
     * Expected:
     * 
     * [ (7,14,) ] 
     * [ (5,) (8,) (15,) ]
     * [ (4,) (5,6,) ] [ (7,) (8,) ] [ (14,) (15,16,) ]
    */
    test_del_1();

    /**
     * ----- Test 2 -----
     * 
     * Set Node/node.h NUM_KEYS 3
     * 
     * Expected:
     * 
     * [ (8,) ] 
     * [ (6,) (10,) ]
     * [ (1,2,5,) (6,7,) ] [ (8,9,) (10,11,12,) ]
    */
    test_del_2();
}