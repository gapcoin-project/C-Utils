#define VERBOSE 1
#include "../src/red-black-tree.h"

#define REFRESH  12345

/**
 * returns the human readable Bytes as a String
 */
char* readable_fs(u_int64_t size, char *buf) {
  int i = 0;
  u_int64_t comma = 0;
  const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
  while (size > 1024) {
      comma = size % 1024;
      size /= 1024;
      i++;
  }
  sprintf(buf, "%" PRIu64 ".%" PRIu64 " %s", size, comma, units[i]);
  return buf;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("%s <length>\n", argv[0]);
    exit(1);
  }
  int length = atoi(argv[0]);

  u_int64_t space = length * sizeof(Node) + sizeof(RBTree);
  char buf[100];
  printf("Working with: %d Objects needing %" PRIu64  " Bytes Space: %s\n", 
          length, space, readable_fs(space, buf));

  RBTree tree;
  init_rbtree(&tree, length);
  u_int64_t i;

  for (i = 0; i < length; i++) {
    rbtree_add(&tree, i);
    if (i % REFRESH == 0)
      printf("Adding\t\t %10" PRIu64 "    \r", length - i);
  }
  printf("\n");

  for (i = 0; i < length; i++) {
    if (rbtree_contains(&tree, i) == RBT_FALSE)
      printf("[DEBUG] search faile on Key %" PRIu64 "\n", i);
      
    if (i % REFRESH == 0)
      printf("Searching\t %10" PRIu64 "    \r", length - i);
  }
  printf("\n");

  for (i = 0; i < length; i++) {
    if (rbtree_remove(&tree, i) == 0) 
      printf("[DEBUG] remove faile on Key %" PRIu64 "\n", i);

    if (i % REFRESH == 0)
      printf("Removing\t %10" PRIu64 "    \r", length - i);
  }
  printf("\n");

  rbtree_free(&tree);

  return 0;
}
