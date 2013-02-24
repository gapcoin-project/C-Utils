#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/bool-matrix.h"

int main(int argc, char *argv[]) {
  
  if (argc != 2) {
    printf("%s <matrix size>\n", argv[0]);
    exit(1);
  }

  uint32_t n = atoi(argv[1]);
  srand(time(0));


  BoolMatrix m;
  INIT_MATRIX(m, n ,n);
  MATRIX_SET_MAIN_DIAGONAL_PART(m, n);

  uint32_t i, j;
  for (i = 0; i < n/2; i++) {
    uint32_t x, y;
    x = rand()%n;
    y = rand()%n;
    MATRIX_SET_BOTH(m, x, y);
  }

  printf("  |");
  for (i = 0; i < n; i++)
    printf("%2d", i);

  printf("\n--+");
  for (i = 0; i < n; i++)
    printf("--");

  for (i = 0; i < n; i++) {
    printf("\n%2d|", i);
    for (j = 0; j < n; j++)
      printf("%2d", MATRIX_AT(m, j, i));
  }
  printf("\n\n");

  MATRIX_CALC_PATHS_WITHOUT(m, n);

  printf("  |");
  for (i = 0; i < n; i++)
    printf("%2d", i);

  printf("\n--+");
  for (i = 0; i < n; i++)
    printf("--");

  for (i = 0; i < n; i++) {
    printf("\n%2d|", i);
    for (j = 0; j < n; j++)
      printf("%2d", MATRIX_AT(m, j, i));
  }
  printf("\n\n");

  return 0;
}
