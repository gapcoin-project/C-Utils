#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include "../src/bool-matrix.h"

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("%s <matrix size>\n", argv[0]);
    exit(1);
  }

  u_int32_t n = atoi(argv[1]);
  srand(time(0));


  BoolMatrix m;
  INIT_MATRIX(m, n ,n);

  u_int32_t i, j, x, y;
  while (1) {
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
    for (i = 0; i < (n/8+1); i++) {
      for (j = 0; j < n; j++) {
        printf("%02X ", m.ptr[j][i/8]);
      }
      printf("\n");
    }
    printf("\n\nEingabe: ");
    scanf("%d %d", &x, &y);
    MATRIX_SET(m, x, y);
    printf("\n");
  }

  return 0;
}
