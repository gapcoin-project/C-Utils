/**
 * These File contains an implementaion of an Red Black Tree
 * and its operation fuctions
 */
#ifndef __RED_BLACK_TREE__
#define __RED_BLACK_TREE__
#include "red-black-tree.h"
#include <math.h>
#include "../../String/src/string.h"

/**
 * static functions
 */
static inline void rbt_delete_case6(RBTree *tree, RBTNode *n);
static inline void rbt_delete_case5(RBTree *tree, RBTNode *n);
static inline void rbt_delete_case4(RBTree *tree, RBTNode *n);
static inline void rbt_delete_case3(RBTree *tree, RBTNode *n);
static inline void rbt_delete_case2(RBTree *tree, RBTNode *n);
static inline void rbt_delete_case1_nil_node(RBTree *tree, RBTNode *n);
static void rbt_delete_case1(RBTree *tree, RBTNode *n);
static inline void rbt_free_node(RBTree *tree, RBTNode *n);
static inline void rbt_repleace(RBTree *tree, RBTNode *dst, RBTNode *src);
static inline void rbt_delete_one_child(RBTree *tree, RBTNode *n);
static inline void rbt_insert_case5(RBTree *tree, RBTNode *n);
static inline void rbt_insert_case4(RBTree *tree, RBTNode *n);
static inline void rbt_insert_case3(RBTree *tree, RBTNode *n);
static inline void rbt_insert_case2(RBTree *tree, RBTNode *n);
static void rbt_insert_case1(RBTree *tree, RBTNode *n);
static void rbt_rotate_left(RBTree *tree, RBTNode *n);
static void rbt_rotate_right(RBTree *tree, RBTNode *n);
static inline RBTNode *rbt_search(RBTree *tree, uint64_t key);

/**
 * Initializes an given Red-Back-Tree with given
 * max number of a given RBTNode
 */
void init_rbtree(RBTree *tree, uint64_t max_nodes) {

  tree->root = NULL;                         
  tree->cur  = NULL;
  ARY_INIT(RBTNode, tree->nodes, max_nodes);

}

/**
 * Searches for a given key and Returns RBT_TRUE or RBT_FALSE
 * if the given RBTree contains the key or not
 */
uint8_t rbtree_contains(RBTree *tree, uint64_t key) {
  RBTNode *cur = tree->root;

  while (cur != NULL) {
    if (cur->key == key)
      return RBT_TRUE;

    if (key < cur->key)
      cur = cur->left;
    else
      cur = cur->right;
  }

  return RBT_FALSE;
}

/**
 * Searches for a given key and Returns the RBTNode
 * containing that key, or NULL if there is no such key
 */
static inline RBTNode *rbt_search(RBTree *tree, uint64_t key) {
  RBTNode *cur = tree->root;

  while (cur != NULL) {
    if (cur->key == key)
      return cur;

    if (key < cur->key)
      cur = cur->left;
    else
      cur = cur->right;
  }

  return NULL;
}

/**
 * Returns the grand father of the given node
 */
RBTNode *grandfather(RBTNode *n) {
  return n->father->father;
}

/**
 * Returns the uncle of the given node 
 */
RBTNode *uncle(RBTNode *n) {
  if (n->father == grandfather(n)->left)
    return grandfather(n)->right;
  else
    return grandfather(n)->left;
}

/**
 * Returns the brother of a given RBTNode
 */
RBTNode *brother(RBTNode *n) {
  if (n->father != NULL) {
    if (n == n->father->left)
      return n->father->right;
    else
      return n->father->left;

  } else
    return NULL;
}

/**
 * Processes an right rotation on the given RBTNode
 */
static void rbt_rotate_right(RBTree *tree, RBTNode *n) {
  RBTNode *l = n->left;
  
  n->left = l->right;
  if (n->left != NULL)
    n->left->father = n;

  l->right = n;
  l->father = n->father;

  if (n->father == NULL)
    tree->root = l;
  else if (n == n->father->left)
    n->father->left = l;
  else
    n->father->right = l;

  n->father = l;
}

/**
 * Processes an left rotation on the given RBTNode
 */
static void rbt_rotate_left(RBTree *tree, RBTNode *n) {
  RBTNode *r = n->right;
  
  n->right = r->left;
  if (n->right != NULL)
    n->right->father = n;

  r->left = n;
  r->father = n->father;

  if (n->father == NULL)
    tree->root = r;
  else if (n == n->father->left)
    n->father->left = r;
  else
    n->father->right = r;

  n->father = r;
}

/**
 * Adds a given key to the given RBTree
 */
void rbtree_add(RBTree *tree, uint64_t key, void *value) {

  // reserving memory for next element
  ARY_GROW(tree->nodes, ARY_LEN(tree->nodes));

  // pointer to the last element
  RBTNode *new_node = ARY_PTR(tree->nodes, ARY_LEN(tree->nodes));
  
  new_node->key    = key;
  new_node->value  = value;
  new_node->left   = NULL;
  new_node->right  = NULL;
  new_node->father = NULL;
  new_node->color  = RBT_RED;

  // insert new node into the tree
  if (tree->root == NULL)
    tree->root = new_node;
  else {
    RBTNode *cur = tree->root;
    while(cur->key != key) {
 
      if (key < cur->key) {
        if (cur->left == NULL) {
          cur->left = new_node;
          new_node->father = cur;
          break;
 
        } else 
          cur = cur->left;
 
      } else {
        if (cur->right == NULL) {
          cur->right = new_node;
          new_node->father = cur;
          break;
 
        } else 
          cur = cur->right;
 
      }
    }
  }

  // repair red black tree functionality if neccessary
  rbt_insert_case1(tree, new_node);
  tree->nodes.length++;
}

/**
 * Adds a given key to the given RBTree if it not alreday in it
 * returns RBT_TRUE on success, else RBT_FALSE
 */
uint8_t rbtree_add_if_possible(RBTree *tree, uint64_t key, void *value) {

  // reserving memory for next element
  ARY_GROW(tree->nodes, ARY_LEN(tree->nodes));

  // pointer to the last element
  RBTNode *new_node = ARY_PTR(tree->nodes, ARY_LEN(tree->nodes));
  
  new_node->key    = key;
  new_node->value  = value;
  new_node->left   = NULL;
  new_node->right  = NULL;
  new_node->father = NULL;
  new_node->color  = RBT_RED;

  // insert new node into the tree
  if (tree->root == NULL)
    tree->root = new_node;
  else {
    RBTNode *cur = tree->root;
    while(cur->key != key) {
 
      if (key < cur->key) {
        if (cur->left == NULL) {
          cur->left = new_node;
          new_node->father = cur;
          break;
 
        } else 
          cur = cur->left;
 
      } else if (key > cur->key) {
        if (cur->right == NULL) {
          cur->right = new_node;
          new_node->father = cur;
          break;
 
        } else 
          cur = cur->right;
 
      } else // key alredy in this three
        return RBT_FALSE;
    }
  }

  // repair red black tree functionality if neccessary
  rbt_insert_case1(tree, new_node);
  tree->nodes.length++;
  return RBT_TRUE;
}

/**
 * Insert Case 1: the new RBTNode is the root
 */
static void rbt_insert_case1(RBTree *tree, RBTNode *n) {

  if (n->father == NULL)
    n->color = RBT_BLACK;
  else
    rbt_insert_case2(tree, n);
}

/**
 * Insert Case 2: the father of the new RBTNode is black
 */
static inline void rbt_insert_case2(RBTree *tree, RBTNode *n) {
  if (n->father->color == RBT_BLACK)
    return;
  else
    rbt_insert_case3(tree, n);
}

/**
 * Insert Case 3: uncle and father of the current node are red
 */
static inline void rbt_insert_case3(RBTree *tree, RBTNode *n) {
  if (uncle(n) != NULL && uncle(n)->color == RBT_RED) {
    n->father->color      = RBT_BLACK;
    uncle(n)->color       = RBT_BLACK;
    grandfather(n)->color = RBT_RED;
    rbt_insert_case1(tree, grandfather(n));
  } else
    rbt_insert_case4(tree, n);
}

/**
 * Insert Case 4: the currend node has no or an black uncle and
 *
 *  is the right child of his father which is the left child or
 *  is the left  child of his father which is the right child
 */
static inline void rbt_insert_case4(RBTree *tree, RBTNode *n) {
  if (n == n->father->right && n->father == grandfather(n)->left) {
    rbt_rotate_left(tree, n->father);
    n = n->left;
  } else if (n == n->father->left && n->father == grandfather(n)->right) {
    rbt_rotate_right(tree, n->father);
    n = n->right;
  }
  rbt_insert_case5(tree, n);
}

/**
 * Insert Case 4: the currend node has no or an black uncle and
 *
 *  is the right child of his father which is the right child or
 *  is the left  child of his father which is the left  child
 */
static inline void rbt_insert_case5(RBTree *tree, RBTNode *n) {
  n->father->color      = RBT_BLACK;
  grandfather(n)->color = RBT_RED;

  if (n == n->father->left && n->father == grandfather(n)->left)
    rbt_rotate_right(tree, grandfather(n));
  else
    rbt_rotate_left(tree, grandfather(n));  
}

/**
 * Removes a given key from the given RBTree
 *
 * returns RBT_TRUE or RBT_FALSE if the key could be removed
 */
uint8_t rbtree_remove(RBTree *tree, uint64_t key) {
  RBTNode *n = rbt_search(tree, key);

  if (n == NULL) {
    #ifdef VERBOSE
      printf("there is no such key %" PRIu64 " to remove\n", key);
    #endif

    return RBT_FALSE;
  }

  if (n->left != NULL && n->right != NULL) {
    RBTNode *cur = n->right;
    
    while (cur->left != NULL)
      cur = cur->left;

    // switch n and cur
    n->key = cur->key;
    n = cur;
  }

  rbt_delete_one_child(tree, n);

  return RBT_TRUE;
}

/**
 * Deletes an RBTNode which have onely one right child
 */
static inline void rbt_delete_one_child(RBTree *tree, RBTNode *n) {

  RBTNode *child = (n->left != NULL) ? n->left : n->right;
  rbt_repleace(tree, n, child);

  if (n->color == RBT_BLACK) {
    if (child != NULL && child->color == RBT_RED) 
      child->color = RBT_BLACK;
    else if (child != NULL)
      rbt_delete_case1(tree, n->right);
    else 
      rbt_delete_case1_nil_node(tree, n);
  }
  
  rbt_free_node(tree, n);
}

/**
 * repleaces the first given node with the second  nodes in the given RBTree
 *
 * Note this function should one be called by rbt_delete_one_child
 * because it guesses that src is a chield of dst
 */
static inline void rbt_repleace(RBTree *tree, RBTNode *dst, RBTNode *src) {

  // repleace dst with src
  if (src != NULL)
    src->father = dst->father;

  if (dst->father != NULL) {  
    if (dst == dst->father->left)
      dst->father->left = src;
    else
      dst->father->right = src;

  } else
    tree->root = src;

}

/**
 * frees a given RBTNode in the given RBTree
 *
 * Note it doesn't frees children of the given node
 * 
 * NOTE: the RBTNode should be first undocked from the rbtree
 * bevor calling this
 */
static inline void rbt_free_node(RBTree *tree, RBTNode *n) {

#ifdef DEBUG
  if (n->father != NULL) {
    if (n == n->father->left)
      printf("[DEBUG] rbt_free_node: left father not undocked\n");
    else if (n == n->father->right)
      printf("[DEBUG] rbt_free_node: right father not undocked\n");
  }
#endif
  
  tree->nodes.length--;
  RBTNode *last_node = ARY_PTR(tree->nodes, tree->nodes.length);

  // copy last element from the RBTNodes Array to dst
  if (n != last_node) {
    *n = *last_node;
 
    // reparing pointer
    if (n->left != NULL)
      n->left->father = n;
 
    if (n->right != NULL)
      n->right->father = n;
 
    if (n->father != NULL) {
      // pointer to last elemnt which is now dst
      if (last_node == n->father->left)
        n->father->left = n;
      else if (last_node == n->father->right)
        n->father->right = n;
      else
        printf("[DEBUG] something whent worong in rbt_free_node!\n");
    } else 
      tree->root = n;
  }

  if (tree->free_value)
    free(n->value);
}

/**
 *  Delete Case 1: The new RBTNode is the root
 */
static void rbt_delete_case1(RBTree *tree, RBTNode *n) {
  if (n->father == NULL)
    return;
  else
    rbt_delete_case2(tree, n);
}

/**
 * Delete Case 1_nil_node
 */
static inline void rbt_delete_case1_nil_node(RBTree *tree, RBTNode *n) {

  if (n->father == NULL)
    return;
  else {

    // create Nill RBTNode
    RBTNode nil;
    nil.father = n->father;
    nil.left   = nil.right = NULL;
    nil.color  = RBT_BLACK;
 
    // a black node always have an brother
    if (nil.father->left == NULL)
      nil.father->left = &nil;
    else
      nil.father->right = &nil;
 
    rbt_delete_case2(tree, &nil);
 
    // remove nil node
    if (nil.father->left == &nil)
      nil.father->left = NULL;
    else if (nil.father->right == &nil)
      nil.father->right = NULL;
    else 
      printf("[DEBUG] Some thing went wrong in rbt_delete_case1_nil_node!\n");

  }

}

/**
 * Delete Case 2: The Borther of n is red
 */
static inline void rbt_delete_case2(RBTree *tree, RBTNode *n) {
  if (brother(n) != NULL && brother(n)->color == RBT_RED) {
    n->father->color  = RBT_RED;
    brother(n)->color = RBT_BLACK;
    if (n == n->father->left)
      rbt_rotate_left(tree, n->father);
    else if (n == n->father->right)
      rbt_rotate_right(tree, n->father);
    else 
      printf("[DEBUG] something wrent wrong in rbt_delete_case2!\n");
  }

  rbt_delete_case3(tree, n);
}

/**
 * Delete Case 3: The Father of n his brother and 
 * the children of his brother are black
 */
static inline void rbt_delete_case3(RBTree *tree, RBTNode *n) {
  if ( n->father->color == RBT_BLACK 
       && brother(n) != NULL 
       && brother(n)->color == RBT_BLACK 
       && ( brother(n)->left == NULL 
            || brother(n)->left->color == RBT_BLACK ) 
       && ( brother(n)->right == NULL 
            || brother(n)->right->color == RBT_BLACK ) ) {

    brother(n)->color = RBT_RED;
    rbt_delete_case1(tree, n->father);
  } else
    rbt_delete_case4(tree, n);
}

/**
 * Delete Case 4: the brother and the children of 
 * the brother of n are black, bu the father of n are red
 */
static inline void rbt_delete_case4(RBTree *tree, RBTNode *n) {
  if ( n->father->color == RBT_RED 
       && brother(n) != NULL 
       && brother(n)->color == RBT_BLACK 
       && ( brother(n)->left == NULL 
            || brother(n)->left->color == RBT_BLACK ) 
       && ( brother(n)->right == NULL 
            || brother(n)->right->color == RBT_BLACK ) ) {

    brother(n)->color = RBT_RED;
    n->father->color  = RBT_BLACK;
  } else
    rbt_delete_case5(tree, n);
}

/**
 * Delete Case 5: One child of the brother of n is red 
 * the other and the brother of n are black
 * TODO correct describtion
 */
static inline void rbt_delete_case5(RBTree *tree, RBTNode *n) {
  if ( n == n->father->left 
       && brother(n) != NULL 
       && brother(n)->color == RBT_BLACK 
       && brother(n)->left != NULL 
       && brother(n)->left->color == RBT_RED 
       && ( brother(n)->right == NULL 
            || brother(n)->right->color == RBT_BLACK ) ) {

    brother(n)->color = RBT_RED;
    brother(n)->left->color = RBT_BLACK;
    rbt_rotate_right(tree, brother(n));

  } else if ( n == n->father->right 
              && brother(n) != NULL && brother(n)->color == RBT_BLACK 
              && brother(n)->right != NULL 
              && brother(n)->right->color == RBT_RED 
              && ( brother(n)->left == NULL 
                   || brother(n)->left->color == RBT_BLACK ) ) {

    brother(n)->color = RBT_RED;
    brother(n)->right->color = RBT_BLACK;
    rbt_rotate_left(tree, brother(n));
  }

  rbt_delete_case6(tree, n);
}

/**
 * Delete Case 6: One child of the brother of n is red the other 
 * and the brother of n are black
 * TODO correct describtion
 */
static inline void rbt_delete_case6(RBTree *tree, RBTNode *n) {
  brother(n)->color = n->father->color;
  n->father->color  = RBT_BLACK;

  if (n == n->father->left) {
    brother(n)->right->color = RBT_BLACK;
    rbt_rotate_left(tree, n->father);

  } else if (n == n->father->right) {
    brother(n)->left->color = RBT_BLACK;
    rbt_rotate_right(tree, n->father);

  } else 
    printf("[DEBUG] something went wrong in rbt_delete_case6!\n");
}

/**
 * frees an given RedBlackTree
 */
void rbtree_free(RBTree *tree) {
  ARY_FREE(tree->nodes);
}


/**
 * Clones a given RBTree fromsrc into dst
 */
void rbtree_clone(RBTree *dst, RBTree *src) {
  dst->root = src->root;
  ARY_CLONE(dst->nodes, src->nodes);
}

/**
 * Returns the Maximum key in the given RBTree
 */
uint64_t rbtree_max(RBTree *tree) {
  RBTNode *cur = tree->root;

  if (cur == NULL)
    return 0;

  while (cur->right != NULL)
    cur = cur->right;

  return cur->key;
}

/**
 * Returns the Minimum key in the given RBTree
 */
uint64_t rbtree_min(RBTree *tree) {
  RBTNode *cur = tree->root;

  if (cur == NULL)
    return 0;

  while (cur->left != NULL)
    cur = cur->left;

  return cur->key;
}

/**
 * progress a deep first search to fill a sorted key array
 */
static void rbt_to_key_ary(RBTNode *n, Uint64Ary *ary) {
  
  if (n->left != NULL)
    rbt_to_key_ary(n->left, ary);

  ARY_PUSH(*ary, n->key);

  if (n->right != NULL)
    rbt_to_key_ary(n->right, ary);

}

/**
 * converts the given RBTree to an sorted value array
 */
Uint64Ary *rbtree_to_key_ary(RBTree *tree) {
  
  Uint64Ary *ary = malloc(sizeof(Uint64Ary));
  ARY_INIT(uint64_t, *ary, ARY_LEN(tree->nodes));

  rbt_to_key_ary(tree->root, ary);

  return ary;
}

/**
 * progress a deep first search to fill a sorted value array
 * (soretd by keys)
 */
static void rbt_to_value_ary(RBTNode *n, VoidPtrAry *ary) {
  
  if (n->left != NULL)
    rbt_to_value_ary(n->left, ary);

  ARY_PUSH(*ary, n->value);

  if (n->right != NULL)
    rbt_to_value_ary(n->right, ary);

}

/**
 * converts the given RBTree to an sorted value array
 */
VoidPtrAry *rbtree_to_value_ary(RBTree *tree) {
  
  VoidPtrAry *ary = malloc(sizeof(VoidPtrAry));
  ARY_INIT(void *, *ary, ARY_LEN(tree->nodes));

  rbt_to_value_ary(tree->root, ary);

  return ary;
}

/**
 * starts an iterationg over the given RBTree
 */
void rbtree_start_iteration(RBTree *tree) {

  if (tree->root == NULL)
    tree->cur = NULL;
  else
    for (tree->cur = tree->root; 
       tree->cur->left != NULL; 
       tree->cur = tree->cur->left);

}

/**
 * Gos on to the next node in the itteration
 */
void rbtree_iteration_next(RBTree *tree) {
  
  if (tree->cur->right != NULL) {

    for (tree->cur = tree->cur->right; 
         tree->cur->left != NULL;
         tree->cur = tree->cur->left);

  } else if (tree->cur == tree->root) {
    tree->cur = NULL;
  
  } else if (tree->cur->father->left == tree->cur) {
    tree->cur = tree->cur->father;
  
  } else {
    for (; tree->cur != tree->root && 
           tree->cur->father->right == tree->cur;
           tree->cur = tree->cur->father);

    if (tree->cur != tree->root)
      tree->cur = tree->cur->father;
    else
      tree->cur = NULL;
  }
}

/**
 * prints an rbtree from a given note on
 */
void rbt_print(RBTNode *n, RBTNodeAry *ary, int deep) {
  
  ARY_PUSH(ary[deep], *n);

  if (n->right != NULL)
    rbt_print(n->right, ary, deep + 1);

  if (n->left != NULL)
    rbt_print(n->left, ary, deep + 1);

}

/**
 * prints an red black tree
 */
void rbtree_print(RBTree *tree) { 

  uint64_t len = log2(ARY_LEN(tree->nodes)) * 2;
  RBTNodeAry *ary = malloc(sizeof(RBTNodeAry) * len);

  uint64_t i, j;
  for (i = 0; i < len; i++)
    ARY_INIT(RBTNode, ary[i], (i + 1) * (i + 1));

  rbt_print(tree->root, ary, 0);


  for (i = 0; i < len; i++) {
    for (j = 0; j < ARY_LEN(ary[i]); j++) {

      printf("%" PRIu64 ":(%s, %s) ",
             ARY_AT(ary[i], j).key,
             (ARY_AT(ary[i], j).right != NULL) ? 
              itoa(ARY_AT(ary[i], j).right->key) : "N",
             (ARY_AT(ary[i], j).left != NULL) ? 
              itoa(ARY_AT(ary[i], j).left->key) : "N");
    }

    printf("\n");
  }
}


#endif // __RED_BLACK_TREE__
