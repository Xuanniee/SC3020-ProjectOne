#include <tuple>
#include <vector>
#include "../Node/node.h"


/**
 * @brief Update the first left parent key with smallest value in current subtree
 * 
 * @param st Ancestry stack
 * @param key Smallest key value
*/
void _updateFirstLeft( std::vector<std::pair<Node*, int> > st, unsigned short int key);


/**
 * @brief Find a leaf sibling that can have afford to borrow 1 key
 * 
 * @param parent Parent node
 * @param offset Current leaf node's offset within parent's children
 * 
 * @return Offset of sibling if exists, else -1 
*/
int _leafSibling(InternalNode* parent, int offset);


/**
 * @brief Find a sibling that can have afford to borrow 1 key
 * 
 * @param parent Parent node
 * @param offset Current node's offset within parent's children
 * 
 * @return Offset of sibling if exists, else -1 
*/
int _sibling(InternalNode* parent, int offset);


/**
 * @brief Shift keys & pointers
 * 
 * @param node Internal node
 * @param dir Direction
 * @param src Source index

*/
void _shift(InternalNode* node, int src, int dir);


/**
 * @brief Shift keys & pointers
 * 
 * @param node Leaf node 
 * @param src Source index
 * @param dir Direction
*/
void _shift(LeafNode* node, int src, int dir);


/**
 * @brief Find leftmost key value from a parent node
 * 
 * @param node Parent node
 * 
 * @return Key value
*/
unsigned short int _leftmost(Node* node);