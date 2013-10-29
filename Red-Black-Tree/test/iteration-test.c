#include "../src/red-black-tree.h"
#include "../../Test/src/test.h"
#include <time.h>
#include <stdlib.h>

#define RBT_LEN 10

BEVOR() {
  
  (void) args;

  srand(time(NULL));

  RBTree *tree = malloc(sizeof(RBTree));
  init_rbtree(tree, RBT_LEN);

  int i;
  for (i = 0; i < RBT_LEN; i++)
    rbtree_add(tree, rand() % RBT_LEN);

  return (void *) tree;
}

TEST(test_iteration) {

  RBTree *tree = (RBTree *) args;

  Uint64Ary *kys = rbtree_to_key_ary(tree);

  

  unsigned int i;
  TEST_MSG5("[II] ary:\n");
  for (i = 0; i < ARY_LEN(*kys); i++)
    TEST_MSG5("[II] %u: %" PRIu64 "\n", i, ARY_AT(*kys, i));


  TEST_MSG5("\n[II] it:\n");
  for (i = 0, rbtree_start_iteration(tree); 
       !rbtree_iteration_finished(tree); 
       rbtree_iteration_next(tree), i++) {

    TEST_MSG5("[II] %u: %" PRIu64 "\n", i, rbtree_cur_key(tree));
    ASSERT_TRUE(ARY_AT(*kys, i) == rbtree_cur_key(tree), 
                "[EE] ary[%d]: %" PRIu64 ", it: %" PRIu64 "\n",
                i,
                ARY_AT(*kys, i),
                rbtree_cur_key(tree));
  }

  if (tunit.verbose == 5)
    rbtree_print(tree);

  return (void *) tree;
}

AFTER() {

  RBTree *tree = (RBTree *) args;

  rbtree_free(tree);

}
