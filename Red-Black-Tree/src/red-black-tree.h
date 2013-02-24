/**
 * Header of an implementaion of an Red Black Tree
 *
 * Note this is a smal implementation with onley uint64_t keys
 * in each node, bute someone could later easyly add void pointer maybee
 */
#include <sys/types.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "LongArray.h"

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
  uint64_t key;
};

/**
 * Long Array for storing the RBTNodes
 */
DEF_LARY(RBTNode, RBTNodeAry)

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

// functions
void init_rbtree(RBTree *tree, uint64_t max_nodes);
uint8_t rbtree_contains(RBTree *tree, uint64_t key);
RBTNode *grandfather(RBTNode *n);
RBTNode *uncle(RBTNode *n);
RBTNode *brother(RBTNode *n);
void rbt_rotate_right(RBTree *tree, RBTNode *n);
void rbt_rotate_left(RBTree *tree, RBTNode *n);
void rbtree_add(RBTree *tree, uint64_t key);
uint8_t rbtree_add_if_possible(RBTree *tree, uint64_t key);
void rbt_insert_case1(RBTree *tree, RBTNode *n);
void rbt_insert_case2(RBTree *tree, RBTNode *n);
void rbt_insert_case3(RBTree *tree, RBTNode *n);
void rbt_insert_case4(RBTree *tree, RBTNode *n);
void rbt_insert_case5(RBTree *tree, RBTNode *n);
uint8_t rbtree_remove(RBTree *tree, uint64_t key);
void rbt_delete_one_child(RBTree *tree, RBTNode *n);
void rbt_repleace(RBTree *tree, RBTNode *dst, RBTNode *src);
void rbt_free_node(RBTree *tree, RBTNode *n);
void rbt_delete_case1(RBTree *tree, RBTNode *n);
void rbt_delete_case1_nil_node(RBTree *tree, RBTNode *n);
void rbt_delete_case2(RBTree *tree, RBTNode *n);
void rbt_delete_case3(RBTree *tree, RBTNode *n);
void rbt_delete_case4(RBTree *tree, RBTNode *n);
void rbt_delete_case5(RBTree *tree, RBTNode *n);
void rbt_delete_case6(RBTree *tree, RBTNode *n);
void rbtree_free(RBTree *tree);
void rbtree_clone(RBTree *dst, RBTree *src);
uint64_t rbtree_max(RBTree *tree);
uint64_t rbtree_min(RBTree *tree);
