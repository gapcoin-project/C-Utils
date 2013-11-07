/**
 * Header of an implementaion of an Red Black Tree
 *
 * Note this is a smal implementation with onley uint64_t keys
 * in each node, bute someone could later easyly add void pointer maybee
 */
#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__
#include <sys/types.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../Dynamic-Array/src/dynamic-array.h"

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
 * uint64_t long array
 * and void pinter long ary
 */
DEF_LARY(uint64_t, Uint64Ary);
DEF_LARY(void *, VoidPtrAry);

/**
 * RBTNode for an Red-Balck-Tree
 */
typedef struct RBTNode RBTNode;     
struct RBTNode {                 
  RBTNode *left, *right, *father;   
  char color;                 
  uint64_t key;
  void *value;
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
  RBTNode *cur;     /* used for itterating */
  RBTNodeAry nodes;                              
  char free_value;
} RBTree;

/**
 * returns the length of an red black tree
 */
#define rbtree_length(tree) (LARY_LEN((tree)->nodes))

/**
 * Clears an RBTree (sets the num of elements to zero)
 */
#define RBTREE_CLEAR(TREE) TREE.nodes.length = 0

/**
 * Initializes an given Red-Back-Tree with given
 * max number of a RBTNode
 */
void init_rbtree(RBTree *tree, uint64_t max_nodes);

/**
 * Searches for a given key and Returns RBT_TRUE or RBT_FALSE
 * if the given RBTree contains the key or not
 */
uint8_t rbtree_contains(RBTree *tree, uint64_t key);

/**
 * to use rbtree_add without giving a value
 * (backward compatibility)
 */
#define rbtree_add1(tree, key) rbtree_add(tree, key, NULL)
#define rbtree_add2(tree, key, value) rbtree_add(tree, key, value)
#define rbtree_addx(X, Y, A, B, F, ...) F
#define rbtree_add(...) rbtree_addx(, ##__VA_ARGS__,            \
                                    rbtree_add2(__VA_ARGS__),   \
                                    rbtree_add1(__VA_ARGS__))

/**
 * Adds a given key to the given RBTree
 */
void rbtree_add(RBTree *tree, uint64_t key, void *value);

/**
 * to use rbtree_add_if_possible without giving a value
 * (backward compatibility)
 */
#define rbtree_add_if_possible1(tree, key) \
  rbtree_add_if_possible(tree, key, NULL)
#define rbtree_add_if_possible2(tree, key, value) \
  rbtree_add_if_possible(tree, key, value)
#define rbtree_add_if_possiblex(X, Y, A, B, F, ...) F
#define rbtree_add_if_possible(...)                               \
  rbtree_add_if_possiblex(, ##__VA_ARGS__,                        \
                          rbtree_add_if_possible2(__VA_ARGS__),   \
                          rbtree_add_if_possible1(__VA_ARGS__))

/**
 * Adds a given key to the given RBTree if it not alreday in it
 * returns RBT_TRUE on success, else RBT_FALSE
 */
uint8_t rbtree_add_if_possible(RBTree *tree, uint64_t key, void *value);

/**
 * Removes a given key from the given RBTree
 *
 * returns RBT_TRUE or RBT_FALSE if the key could be removed
 */
uint8_t rbtree_remove(RBTree *tree, uint64_t key);

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
uint64_t rbtree_max(RBTree *tree);

/**
 * Returns the Minimum key in the given RBTree
 */
uint64_t rbtree_min(RBTree *tree);

/**
 * converts the given RBTree to an sorted value array
 */
Uint64Ary *rbtree_to_key_ary(RBTree *tree);

/**
 * converts the given RBTree to an sorted value array
 */
VoidPtrAry *rbtree_to_value_ary(RBTree *tree);

/**
 * starts an iterationg over the given RBTree
 */
void rbtree_start_iteration(RBTree *tree);

/**
 * returns the curent key of the iteration
 * if iteration was not initialized an SIGSEGV is raised
 */
#define rbtree_cur_key(tree) (tree)->cur->key
#define rbtree_cur_value(tree) (tree)->cur->value

/**
 * Gos on to the next node in the itteration
 */
void rbtree_iteration_next(RBTree *tree);

/**
 * return wether rbtree_iteration_next has reatch the end of iteration
 * (no more element)
 */
#define rbtree_iteration_finished(tree) ((tree)->cur == NULL)

/**
 * prints an red black tree
 */
void rbtree_print(RBTree *tree);

#endif // __RED_BLACK_TREE_H__
