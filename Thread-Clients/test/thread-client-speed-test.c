#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../src/thread-client.h"
#include "../../Time-Diff/src/time-diff.h"

/**
 * as higher as longe the test take
 */
#define TEST_HARDNES 1000000

typedef struct {
  int a, b, c, d, e, f;
} Vektor;

void *thread_func(void *opts) {
  (void) opts;

  Vektor v;
  v.a = (rand() % 100) - 50;
  v.b = (rand() % 100) - 50;
  v.c = (rand() % 100) - 50;
  v.d = (rand() % 100) - 50;
  v.e = (rand() % 100) - 50;
  v.f = (rand() % 100) - 50;
  
  int i;
  for (i = 0; i < TEST_HARDNES; i++) {
    v.a = v.b + v.e;
    v.f = v.a + v.a;
    v.b = v.e + v.b;
    v.c = v.d + v.d;
    v.d = v.a + v.f;
    v.e = v.b + v.a;
    v.a = v.e + v.b;
    v.d = v.f + v.c;
    v.e = v.c + v.e;
    v.f = v.a + v.d;
    v.a = v.e + v.f;
    v.b = v.f + v.e;
    v.c = v.d + v.c;
    v.f = v.d + v.a;
    v.d = v.a + v.d;
    v.e = v.b + v.e;
    v.c = v.e + v.f;
  }

  return NULL;
}

int main (int argc, char *argv[]) {

  if (argc != 5) {
    printf("%s <times> <threads> <sched policy> <sched priority>\n", argv[0]);
    exit(1);
  }

  srand(time(NULL) * rand());

  int times = atoi(argv[1]);
  int n_threads = atoi(argv[2]);
  times /= n_threads;

  int priority = atoi(argv[4]);
  int policy   = 0;

  if (!strcmp("SCHED_OTHER", argv[3]))
    policy = SCHED_OTHER;
#ifdef SCHED_BATCH
  else if (!strcmp("SCHED_BATCH", argv[3]))
    policy = SCHED_BATCH;
#endif
#ifdef SCHED_IDLE
  else if (!strcmp("SCHED_IDLE", argv[3]))
    policy = SCHED_IDLE;
#endif
  else if (!strcmp("SCHED_FIFO", argv[3]))
    policy = SCHED_FIFO;
  else if (!strcmp("SCHED_RR", argv[3]))
    policy = SCHED_RR;
  else {
    printf("unknowen policy!\n");
    exit(EXIT_FAILURE);
  }

  pthread_t *threads = malloc(sizeof(pthread_t) * n_threads);
  TClient   *clients = malloc(sizeof(TClient)   * n_threads);

  int i, j;
  for (i = 0; i < n_threads; i++) {
    init_thread_client(&clients[i]);
    tc_set_scheduler(&clients[i], policy, priority);
  }

  
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

  for (i = 0; i < n_threads; i++)
    tc_free(&clients[i]);

  free(threads);
  free(clients);


  return 0;

}

