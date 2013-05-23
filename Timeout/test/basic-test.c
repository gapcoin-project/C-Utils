#include <stdio.h>
#include <stdlib.h>
#include "../src/timeout.h"

void do_work(void *buff, void *args) {

  int a, b, i, c;
  a = *(int *) args;

  for (i = 0; i < a; i++) {
    b = rand() % (i + 1);
    c = b % (rand() + 1);
    b = c % (rand() + 1);
  }

  ((int *) buff)[0] = i;


}

int main(int argc, char *argv[]) {
  
  if (argc != 3) {
    printf("%s <timout> <i>\n", argv[0]);
    exit(1);
  }
   
  int i = atoi(argv[2]);
  struct timeval time;
  time.tv_usec = 0;
  time.tv_sec  = atoi(argv[1]);

  int *buffer = (int *) with_timeout_do(&time, do_work, sizeof(int), &i);
  
  if(buffer == NULL)
    printf("Timeout!!\n");
  else
    printf("Index %d\n", *buffer);

  free(buffer);

  return 0;

}
