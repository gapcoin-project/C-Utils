#include "../src/hash.h"
#include <stdio.h>

DEF_HASH(uint32_t, IntHash);

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("%s <num elements>\n", argv[0]);
    exit(1);
  }
    

  uint32_t n_elements = atoi(argv[1]);

  IntHash hash;

  HASH_INIT(hash, 4);

  if (hash.len != 5)
    printf("[DD] prime number error\n");

  uint32_t i, len = hash.len;
  for (i = 0; i < n_elements; i++) {
    HASH_ADD(hash, i, i);
    
    if (len != hash.len) {
      len = hash.len;
      printf("new len: %" PRIu32 "\n", len); 
    }
  }

  uint32_t e = 0;
  for (i = 0; i < n_elements; i++) {
    HASH_GET(hash, i, e);

    if (e != i)
      printf("[DD] hash error %" PRIu32 ", %" PRIu32 "\n", i, HASH_I(hash, i, 0));
  }
    

  printf("\n");
  printf("hash.len = %" PRIu32 "\n", hash.len);
  printf("hash.h1  = %lf\n", hash.h1);
  printf("hash.h2  = %lf\n", hash.h2);

  for (i = 0; i < hash.len && n_elements < 100; i++) {
    printf("[%" PRId32 "]", (int32_t) hash.kys[i]);
    if (hash.kys[i] != (uint32_t) -1)
      printf(" -> %d", hash.ptr[i]);
    printf("\n");
  }

  return 0;
}
