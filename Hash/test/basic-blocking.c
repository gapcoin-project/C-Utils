#include "../src/blocking-hash.h"
#include <stdio.h>

DEF_BHASH(uint32_t, IntHash);

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("%s <num elements> <num forks>\n", argv[0]);
    exit(1);
  }
    

  uint32_t n_elements = atoi(argv[1]);
  uint32_t n_forks    = atoi(argv[2]);

  IntHash hash;

  BHASH_INIT(hash, n_elements);

  uint32_t i;
  
  for (i = 0; i < n_forks; i++)
    fork();

  for (i = 0; i < n_elements; i++) {
    BHASH_ADD(hash, i, i);
    
  }

  uint32_t e = 0;
  for (i = 0; i < n_elements; i++) {
    BHASH_GET(hash, i, e);

    if (e != i) {
      sem_wait(hash.lock);
      printf("[DD] hash error %" PRIu32 ", %" PRIu32 "\n", i, BHASH_I(hash, i, 0));
      sem_post(hash.lock);
    }

    if (n_forks == 0)
      BHASH_RM(hash, i);
  }
    

  sem_wait(hash.lock);
  printf("\n");
  printf("hash.len = %" PRIu32 "\n", hash.len);
  printf("hash.n_e = %" PRIu32 "\n", *hash.n_e);
  printf("hash.h1  = %lf\n", hash.h1);
  printf("hash.h2  = %lf\n", hash.h2);

  for (i = 0; i < hash.len && n_elements < 100; i++) {
    printf("[%" PRId32 "]", (int32_t) hash.kys[i]);
    if (hash.kys[i] != (uint32_t) -1)
      printf(" -> %d", hash.ptr[i]);
    printf("\n");
  }
  sem_post(hash.lock);

  return 0;
}
