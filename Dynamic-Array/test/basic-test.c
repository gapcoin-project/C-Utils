#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include "../src/dynamic-array.h"

DEF_ARY(uint64_t, IntAry);

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("%s <matrix size>\n", argv[0]);
    exit(1);
  }

  uint32_t n = atoi(argv[1]);
  srand(time(0));

  IntAry ary;

  ARY_INIT(uint64_t, ary, 2);

  uint64_t i, x = 0;
  for (i = 0; i < n; i++) 
    ARY_PUSH(ary, rand());

  for (i = 0; i < n; i++) {
    ARY_PULL(ary, x);
    printf("%" PRIu64 "\n", x);
  }

  return 0;

}
