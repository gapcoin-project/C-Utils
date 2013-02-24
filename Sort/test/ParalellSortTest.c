#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define DEBUG
#include "sort.h"
#include "time.c"

int main (int argc, char *argv[]) {

  if (argc != 5) {
    printf("%s <array length> <max min quicksort> <average times> <threads>\n", argv[0]);
    exit(1);
  }

  srand(time(NULL) * rand());

  int length = atoi(argv[1]);
  int max_min_q = atoi(argv[2]);
  int average = atoi(argv[3]);
  int threads = atoi(argv[4]);

  int *ary1 = (int *) malloc(sizeof(int) * length);

  int i, k;

  int sorted = 1;

  for (k = 0; k < average; k++) {

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }
    
 
    paralellsort(ary1, length, max_min_q, threads);
    
 
    for (i = 0; i < length-1; i++) {
      if (ary1[i] > ary1[i+1])
        sorted = 0;
    }
 
    if (length <= 100) {
      for (i = 0; i < length; i++) 
        printf("%d\n", ary1[i]);
    }
  }

  if (sorted == 1)
    printf("Array sorted!\n");
  else
    printf("Array NOT sorted!!!\n");
    

  return 0;
}
