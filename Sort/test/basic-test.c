#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/sort.h"

static inline char bigger(int64_t a, int64_t b) {
  
  return a > b;
}

static inline char smaler(int64_t a, int64_t b) {
  
  return a < b;
}

static inline char equal(int64_t a, int64_t b) {
  
  return a == b;
}

static inline char p_bigger(void *a, void *b) {
  
  return (*((int64_t *) a)) > (*((int64_t *) b));
}

static inline char p_smaler(void *a, void *b) {
  
  return (*((int64_t *) a)) < (*((int64_t *) b));
}

static inline char p_equal(void *a, void *b) {
  
  return (*((int64_t *) a)) == (*((int64_t *) b));
}

int main (int argc, char *argv[]) {

  if (argc != 5) {
    printf("%s <array length> <max min quicksort> "
           "<times> <threads>\n", argv[0]);
    exit(1);
  }

  srand(time(NULL) * rand());

  int64_t length = atoi(argv[1]);
  int64_t max_min_q = atoi(argv[2]);
  int64_t average = atoi(argv[3]);
  int64_t n_threads = atoi(argv[4]);

  int64_t *ary1 = malloc(sizeof(int64_t) * length);

  int64_t i, k;

  int64_t sorted = 1;
  int64_t sorted_all = 1;
  

  init_tc((uint32_t) log2((double) length), n_threads);
  QISTA_t args;
  args.ary    = (void *) ary1;
  args.base   = sizeof(int64_t);
  args.length = length;
  args.min    = max_min_q;
  args.bigger = p_bigger;
  args.smaler = p_smaler;
  args.equal  = p_equal;

  for (k = 0; k < average; k++) {

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    quickinsersort_min(ary1, 
                       length, 
                       max_min_q, 
                       sizeof(int64_t), 
                       p_smaler, 
                       p_bigger, 
                       p_equal);

    for (i = 0; i < length-1; i++) {
      if (ary1[i] > ary1[i+1]) {
        sorted = 0;
        sorted_all = 0;
      }
    }

    if (!sorted)
      printf("[ERROR] quickinsersort_min FAILED!\n");
    sorted = 1;

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    QUICK_INSERT_SORT_MIN(int64_t,
                          ary1, 
                          length, 
                          bigger, 
                          smaler, 
                          equal, 
                          max_min_q);

    for (i = 0; i < length-1; i++) {
      if (ary1[i] > ary1[i+1]) {
        sorted = 0;
        sorted_all = 0;
      }
    }

    if (!sorted)
      printf("[ERROR] QUICK_INSERT_SORT_MIN FAILED!\n");
    sorted = 1;

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    parallel_quickinsersort_min(&args);


    for (i = 0; i < length-1; i++) {
      if (ary1[i] > ary1[i+1]) {
        sorted = 0;
        sorted_all = 0;
      }
    }

    if (!sorted)
      printf("[ERROR] t_quickinsersort_min FAILED!\n");
    sorted = 1;

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    pt_parallel_quickinsersort_min(&args);


    for (i = 0; i < length-1; i++) {
      if (ary1[i] > ary1[i+1]) {
        sorted = 0;
        sorted_all = 0;
      }
    }

    if (!sorted)
      printf("[ERROR] pthread_quickinsersort_min FAILED!\n");
    sorted = 1;

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    quickinsersort_max(ary1, 
                       length, 
                       max_min_q, 
                       sizeof(int64_t), 
                       p_smaler, 
                       p_bigger, 
                       p_equal);

    for (i = 0; i < length-1; i++) {
      if (ary1[i] < ary1[i+1]) {
        sorted = 0;
        sorted_all = 0;
      }
    }

    if (!sorted)
      printf("[ERROR] quickinsersort_max FAILED!\n");
    sorted = 1;
    
    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    QUICK_INSERT_SORT_MAX(int64_t,
                          ary1, 
                          length, 
                          bigger, 
                          smaler, 
                          equal, 
                          max_min_q);

    for (i = 0; i < length-1; i++) {
      if (ary1[i] < ary1[i+1]) {
        sorted = 0;
        sorted_all = 0;
      }
    }

    if (!sorted)
      printf("[ERROR] QUICK_INSERT_SORT_MAX FAILED!\n");
    sorted = 1;

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    parallel_quickinsersort_max(&args);


    for (i = 0; i < length-1; i++) {
      if (ary1[i] < ary1[i+1]) {
        sorted = 0;
        sorted_all = 0;
      }
    }

    if (!sorted)
      printf("[ERROR] t_quickinsersort_max FAILED!\n");
    sorted = 1;

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    pt_parallel_quickinsersort_max(&args);


    for (i = 0; i < length-1; i++) {
      if (ary1[i] < ary1[i+1]) {
        sorted = 0;
        sorted_all = 0;
      }
    }

    if (!sorted)
      printf("[ERROR] pthread_quickinsersort_max FAILED!\n");
    sorted = 1;

  }

  if (sorted_all)
      printf("Everything went fine, arrays are sorted :-)\n");

  return 0;
}
    
