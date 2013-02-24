#define VERBOSE 1
#include "RedBlackTree.h"

int main() {
  RBTree tree;
  init_rbtree(&tree, 25);

  rbtree_add(&tree, 18);
  rbtree_add(&tree, 1);
  rbtree_add(&tree, 14);
  rbtree_add(&tree, 12);
  rbtree_add(&tree, 23);
  rbtree_add(&tree, 21);
  rbtree_add(&tree, 6);
  rbtree_add(&tree, 5);
  rbtree_add(&tree, 10);
  rbtree_add(&tree, 11);
  rbtree_add(&tree, 3);
  rbtree_add(&tree, 13);
  rbtree_add(&tree, 15);
  rbtree_add(&tree, 8);
  rbtree_add(&tree, 16);
  rbtree_add(&tree, 17);
  rbtree_add(&tree, 22);
  rbtree_add(&tree, 19);
  rbtree_add(&tree, 9);
  rbtree_add(&tree, 2);
  rbtree_add(&tree, 4);
  rbtree_add(&tree, 20);
  rbtree_add(&tree, 7);
  rbtree_add(&tree, 24);
  rbtree_add(&tree, 25);

  printf("%2d => %d \n", 20, rbtree_remove(&tree, 20));
  printf("%2d => %d \n", 2, rbtree_remove(&tree, 2));
  printf("%2d => %d \n", 7, rbtree_remove(&tree, 7));
  printf("%2d => %d \n", 21, rbtree_remove(&tree, 21));
  printf("%2d => %d \n", 3, rbtree_remove(&tree, 3));
  printf("%2d => %d \n", 17, rbtree_remove(&tree, 17));
  printf("%2d => %d \n", 5, rbtree_remove(&tree, 5));
  printf("%2d => %d \n", 6, rbtree_remove(&tree, 6));
  printf("%2d => %d \n", 8, rbtree_remove(&tree, 8));
  printf("%2d => %d \n", 13, rbtree_remove(&tree, 13));
  printf("%2d => %d \n", 9, rbtree_remove(&tree, 9));
  printf("%2d => %d \n", 1, rbtree_remove(&tree, 1));
  printf("%2d => %d \n", 10, rbtree_remove(&tree, 10));
  printf("%2d => %d \n", 12, rbtree_remove(&tree, 12));
  printf("%2d => %d \n", 14, rbtree_remove(&tree, 14));
  printf("%2d => %d \n", 11, rbtree_remove(&tree, 11));
  printf("%2d => %d \n", 25, rbtree_remove(&tree, 25));
  printf("%2d => %d \n", 15, rbtree_remove(&tree, 15));
  printf("%2d => %d \n", 16, rbtree_remove(&tree, 16));
  printf("%2d => %d \n", 23, rbtree_remove(&tree, 23));
  printf("%2d => %d \n", 18, rbtree_remove(&tree, 18));
  printf("%2d => %d \n", 4, rbtree_remove(&tree, 4));
  printf("%2d => %d \n", 19, rbtree_remove(&tree, 19));
  printf("%2d => %d \n", 22, rbtree_remove(&tree, 22));
  printf("%2d => %d \n", 24, rbtree_remove(&tree, 24));

  return 0;
}
