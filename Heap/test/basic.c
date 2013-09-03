#include <time.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/heap.h"

#define FREQ 12345
#define FREQ2 1234

static inline char bigger(long long x, long long y) {
  return x > y;
}

static inline char smaler(long long x, long long y) {
  return x < y;
}

DEF_HEAP(long long, LongHeap);

int main(int argc, char *argv[]) {
  
  if (argc!= 2) {
    printf("%s <num elements>\n", argv[0]);
    exit(1);
  }

  long long a, b, i, n_elements = atoi(argv[1]);
  srand(time(NULL));                                       
  
  LongHeap min_heap, max_heap;
  heap_init(long long, min_heap, 1024, MINHEAP);
  heap_init(long long, max_heap, 1024, MAXHEAP);
  for (i = 0; i < n_elements; i++) {
    if (i % FREQ == 0)
      printf("\r[%lli] adding  \r", n_elements - i);

    heap_add(min_heap, rand()%100, bigger, smaler);
    heap_add(max_heap, rand()%100, bigger, smaler);
  }

  /**
   * testing max_heap 
   */
  heap_extract(max_heap, a, bigger, smaler);
  heap_extract(max_heap, b, bigger, smaler);

  while (heap_notempty(max_heap)) {
    if (max_heap.heap_length % FREQ2 == 0)
      printf("\r[%" PRIu64 "] testing max_heap  \r", max_heap.heap_length);

    if (a < b) {
      printf("\r[DEBUG] max_heap Heap FAILED!\n");
      exit(1);
    }

    heap_extract(max_heap, a, bigger, smaler);
    heap_extract(max_heap, b, bigger, smaler);
  }
  printf("\rmax_heap Heap was valid!!!\n");

  /**
   * testing min_heap 
   */
  heap_extract(min_heap, a, bigger, smaler);
  heap_extract(min_heap, b, bigger, smaler);

  while (heap_notempty(min_heap)) {
    if (min_heap.heap_length % FREQ2 == 0)
      printf("\r[%" PRIu64 "] testing max_heap  \r", min_heap.heap_length);

    if (a > b) {
      printf("\r[DEBUG] min_heap Heap FAILED!\n");
      exit(1);
    }

    heap_extract(min_heap, a, bigger, smaler);
    heap_extract(min_heap, b, bigger, smaler);
  }
  printf("\rmin_heap Heap was valid!!!\n");

  return 0;
}
