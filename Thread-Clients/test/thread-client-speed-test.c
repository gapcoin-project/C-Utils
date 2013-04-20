#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/thread-client.h"
#include "../../Time-Diff/src/time-diff.h"

void *thread_func(void *arg) {

  int *a = (int *) arg;

  (*a)++;

  return (void *) a;

}

int main (int argc, char *argv[]) {

  if (argc != 3) {
    printf("%s <times> <threads>\n", argv[0]);
    exit(1);
  }

  srand(time(NULL) * rand());

  int times = atoi(argv[1]);
  int n_threads = atoi(argv[2]);

  pthread_t *threads = malloc(sizeof(pthread_t) * n_threads);
  TClient   *clients = malloc(sizeof(TClient)   * n_threads);

  int i, j;
  for (i = 0; i < n_threads; i++)
    init_thread_client(&clients[i]);

  
  struct timeval clients_r, pthreads_r; // result
  struct timeval clients_s, pthreads_s; // start
  struct timeval clients_e, pthreads_e; // end

  int a = 0, b = 0;

  gettimeofday(&pthreads_s, NULL);
  for (i = 0; i < times; i++) {

    for (j = 0; j < n_threads; j++)
      pthread_create(&threads[j], NULL, thread_func, (void *) &a);
    
    for (j = 0; j < n_threads; j++) 
      pthread_join(threads[j], NULL);

  }
  gettimeofday(&pthreads_e, NULL);
  timeval_subtract(&pthreads_r, &pthreads_e, &pthreads_s);

  gettimeofday(&clients_s, NULL);
  for (i = 0; i < times; i++) {

    for (j = 0; j < n_threads; j++)
      tc_add_func(&clients[j], thread_func, (void *) &b);
    
    for (j = 0; j < n_threads; j++) 
      tc_join(&clients[j]);

  }
  gettimeofday(&clients_e, NULL);
  timeval_subtract(&clients_r, &clients_e, &clients_s);
  
  
  printf("pthread: ");
  PRINT_TIMEVAL(pthreads_r);
  printf("\n");
  
  printf("clients: ");
  PRINT_TIMEVAL(clients_r);
  printf("\n");

  return 0;

}

