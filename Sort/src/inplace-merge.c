#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sort.h"

void _switch(int64_t *p1, int64_t *p2) {
  int64_t tmp = *p1;
  *p1 = *p2;
  *p2 = tmp;
}

void sort(int64_t *p1, int64_t *p2, int64_t len) {

    int64_t *ptr = p1;

  int64_t *ps = p2;
  int64_t *pe = p2 + len;
  int64_t sc = 0;

  while (p2 != pe) {

    
    if (ps == p1) {
      ps = p2;
      sc = 0;
    }

    int64_t *p = ptr;
    while (p != pe) {
      if (p == p1) printf("  p1  ");
      else if (p == p2 && p == ps) printf(" psp2 ");
      else if (p == p2) printf("  p2  ");
      else if (p == ps) printf("  ps  ");
      else printf("      ");
      p++;
    }
    printf("\n");
    p = ptr;
    while (p != pe) {
      printf("|%3" PRId64 " |", *p);
      p++;
    }
    printf("\n\n");


    if (sc) {
      if (*ps > *p2) {
        _switch(p1, p2);
        p1++;
        p2++;
        sc++;
      } else {
        _switch(p1, ps);
        p1++;
        if (*ps > *(ps + 1)) {
          ps++;
          sc--;
        }
      }

    } else {
      
      if (*p1 > *p2) {
        _switch(p1, p2); 
        p1++;
        p2++;
        sc++;
      } else 
        p1++;

    }
      
  }
    int64_t *p = ptr;
    while (p != pe) {
      if (p == p1) printf("  p1  ");
      else if (p == p2 && p == ps) printf(" psp2 ");
      else if (p == p2) printf("  p2  ");
      else if (p == ps) printf("  ps  ");
      else printf("      ");
      p++;
    }
    printf("\n");
    p = ptr;
    while (p != pe) {
      printf("|%3" PRId64 " |", *p);
      p++;
    }
    printf("\n\n");

}

static inline char bigger(int64_t a, int64_t b) {
  
  return a > b;
}

static inline char smaler(int64_t a, int64_t b) {
  
  return a < b;
}

static inline char equal(int64_t a, int64_t b) {
  
  return a == b;
}

int main (int argc, char *argv[]) {

  if (argc != 2) {
    printf("%s <array length>\n", argv[0]);
    exit(1);
  }

  srand(time(NULL) * rand());

  int64_t length = atoi(argv[1]);

  int64_t *ary1 = malloc(sizeof(int64_t) * length);

  int64_t i;

  int64_t sorted = 1;

  for (i = 0; i < length; i++) {
    ary1[i] = rand() % length;
  }

  QUICK_INSERT_SORT_MIN(int64_t,
                        ary1, 
                        length / 2, 
                        bigger, 
                        smaler, 
                        equal, 
                        20);

  QUICK_INSERT_SORT_MIN(int64_t,
                        ary1 + length / 2, 
                        length / 2, 
                        bigger, 
                        smaler, 
                        equal, 
                        20);

  sort(ary1, ary1 + length / 2, length / 2);
  
  for (i = 0; i < (length / 2) * 2 - 1; i++) {
    if (ary1[i] > ary1[i+1])
      sorted = 0;
  }

  if (sorted == 1)
    printf("Array sorted!\n");
  else
    printf("Array NOT sorted!!!\n");

  return 0;
}
    
