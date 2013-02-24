#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <math.h>
#include "../src/dynamic-array.h"

DEF_ARY(uint64_t, IntAry);

/**
 * returns the human readable Bytes as a String
 */
char* readable_fs(uint64_t size, char *buf) {
  int i = 0;
  uint64_t comma = 0;
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
    printf("%s <matrix size>\n", argv[0]);
    exit(1);
  }

  uint32_t n = atoi(argv[1]);
  srand(time(0));

  char buf[256];
  readable_fs(n * sizeof(uint64_t), buf);
  printf("Expected Size: %s\n", buf);

  IntAry ary;

  ARY_INIT(uint64_t, ary, 2);

  uint64_t i, x = 0;
  for (i = 0; i < n; i++) {
    if (ARY_LEN(ary) == (ary.max_len - 1)) {
      readable_fs((ARY_LEN(ary) + 1) * sizeof(uint64_t), buf);
      printf("[ARY GROW] length = 2^%" PRIu64 "  %15s\n", 
             (uint64_t) log2(ARY_LEN(ary) + 1),
             buf);
    }

    ARY_PUSH(ary, rand());
  }

  for (i = 0; i < n; i++) {
    ARY_PULL(ary, x);
    (void) x;
  }

  return 0;

}
