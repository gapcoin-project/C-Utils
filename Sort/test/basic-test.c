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

  if (argc != 4) {
    printf("%s <array length> <max min quicksort> "
           "<times>\n", argv[0]);
    exit(1);
  }

  srand(time(NULL) * rand());

  int64_t length = atoi(argv[1]);
  int64_t max_min_q = atoi(argv[2]);
  int64_t average = atoi(argv[3]);

  int64_t *ary1 = malloc(sizeof(int64_t) * length);

  int64_t i, k;

  int64_t sorted = 1;

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
      if (ary1[i] > ary1[i+1])
        sorted = 0;
    }

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
      if (ary1[i] > ary1[i+1])
        sorted = 0;
    }

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
      if (ary1[i] < ary1[i+1])
        sorted = 0;
    }
    
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
      if (ary1[i] < ary1[i+1])
        sorted = 0;
    }

  }

  if (sorted == 1)
    printf("Array sorted!\n");
  else
    printf("Array NOT sorted!!!\n");

  return 0;
}
    
