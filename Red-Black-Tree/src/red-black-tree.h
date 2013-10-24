/**
 * Header of an implementaion of an Red Black Tree
 *
 * Note this is a smal implementation with onley rbtree_key_t keys
 * in each node, bute someone could later easyly add void pointer maybee
 */
#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__
#include <sys/types.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../Long-Array/src/long-array.h"

/**
 * the Red Back Tree key type
 */
#ifndef rbtree_key_t
#define rbtree_key_t uint64_t
#endif

/**
 * the Red Black Tree value type
 */
#ifndef rbtree_value_t
#define rbtree_value_t void *
#endif

/**
 * Some Makros
 */
#define RBT_BLACK   'B'
#define RBT_RED     'R'
#define RBT_TRUE    1
#define RBT_FALSE   0
#ifndef NULL
  #define NULL      0
#endif

/**
 * RBTNode for an Red-Balck-Tree
 */
typedef struct RBTNode RBTNode;     
struct RBTNode {                 
  RBTNode *left, *right, *father;   
  char color;                 
  rbtree_key_t key;
  #ifdef RBT_KEY_VALUE
  rbtree_value_t value;
  #endif
};

/**
 * Long Array for storing the RBTNodes
 */
DEF_LARY(RBTNode, RBTNodeAry);

/**
 * The Red-Balck-Tree 
 */
typedef struct {                             
  RBTNode *root;                                
  RBTNodeAry nodes;                              
} RBTree;

/**
 * Clears an RBTree (sets the num of elements to zero)
 */
#define RBTREE_CLEAR(TREE) TREE.nodes.length = 0

/**
 * Initializes an given Red-Back-Tree with given
 * max number of a RBTNode
 */
void init_rbtree(RBTree *tree, rbtree_key_t max_nodes);

/**
 * Searches for a given key and Returns RBT_TRUE or RBT_FALSE
 * if the given RBTree contains the key or not
 */
uint8_t rbtree_contains(RBTree *tree, rbtree_key_t key);

/**
 * Adds a given key to the given RBTree
 */
#ifndef RBT_KEY_VALUE
void rbtree_add(RBTree *tree, rbtree_key_t key);
#else
void rbtree_add(RBTree *tree, rbtree_key_t key, rbtree_value_t value);
#endif

/**
 * Adds a given key to the given RBTree if it not alreday in it
 * returns RBT_TRUE on success, else RBT_FALSE
 */
#ifndef RBT_KEY_VALUE
uint8_t rbtree_add_if_possible(RBTree *tree, rbtree_key_t key);
#else
uint8_t rbtree_add_if_possible(RBTree *tree, 
                               rbtree_key_t key, 
                               rbtree_value_t value);
#endif

/**
 * Removes a given key from the given RBTree
 *
 * returns RBT_TRUE or RBT_FALSE if the key could be removed
 */
uint8_t rbtree_remove(RBTree *tree, rbtree_key_t key);

/**
 * frees an given RedBlackTree
 */
void rbtree_free(RBTree *tree);

/**
 * Clones a given RBTree fromsrc into dst
 */
void rbtree_clone(RBTree *dst, RBTree *src);

/**
 * Returns the Maximum key in the given RBTree
 */
rbtree_key_t rbtree_max(RBTree *tree);

/**
 * Returns the Minimum key in the given RBTree
 */
rbtree_key_t rbtree_min(RBTree *tree);

#endif // __RED_BLACK_TREE_H__
