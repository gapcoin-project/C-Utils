file = File.new "LARGE.c", "w"
file.puts "
#define VERBOSE 1
#define DEBUG   1
#include \"RedBlackTree.h\"

#define LENGTH   #{ ARGV[0] }
#define REFRESH  #{ ARGV[1] }

/**
 * returns the human readable Bytes as a String
 */
char* readable_fs(u_int64_t size, char *buf) {
  int i = 0;
  u_int64_t comma = 0;
  const char* units[] = {\"B\", \"kB\", \"MB\", \"GB\", \"TB\", \"PB\", \"EB\", \"ZB\", \"YB\"};
  while (size > 1024) {
      comma = size % 1024;
      size /= 1024;
      i++;
  }
  sprintf(buf, \"%\" PRIu64 \".%\" PRIu64 \" %s\", size, comma, units[i]);
  return buf;
}

int main() {
  u_int64_t space = LENGTH * sizeof(RBTNode) + sizeof(RBTree);
  char buf[100];
  printf(\"Working with: %d Objects needing %\" PRIu64  \" Bytes Space: %s\\n\", 
          LENGTH, space, readable_fs(space, buf));

  RBTree tree;
  init_rbtree(&tree, LENGTH);
  u_int64_t i;

  for (i = 0; i < LENGTH; i++) {
    rbtree_add(&tree, i);
    if (i % REFRESH == 0)
      printf(\"Adding\\t\\t %10\" PRIu64 \"    \\r\", LENGTH - i);
  }
  printf(\"\\n\");

  for (i = 0; i < LENGTH; i++) {
    if (rbtree_contains(&tree, i) == RBT_FALSE)
      printf(\"[DEBUG] search faile on Key %\" PRIu64 \"\\n\", i);
      
    if (i % REFRESH == 0)
      printf(\"Searching\\t %10\" PRIu64 \"    \\r\", LENGTH - i);
  }
  printf(\"\\n\");

  for (i = 0; i < LENGTH; i++) {
    if (rbtree_remove(&tree, i) == 0) 
      printf(\"[DEBUG] remove faile on Key %\" PRIu64 \"\\n\", i);

    if (i % REFRESH == 0)
      printf(\"Removing\\t %10\" PRIu64 \"    \\r\", LENGTH - i);
  }
  printf(\"\\n\");
  
  rbtree_free(&tree);

  return 0;
}
"

file.close
system "gcc LARGE.c -Wall -lm #{ ARGV[2] }"
