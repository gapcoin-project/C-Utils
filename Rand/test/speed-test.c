#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>


#include "../../Thread-Clients/src/thread-client.h"
#include "../../Time-Diff/src/time-diff.h"
#include "../src/rand.h"

/**
 * thread args
 */
typedef struct {
  uint16_t i, num_threads;
  uint64_t num_rands;
} ThreadArgs;


void *rand32_new(void *opts) {
  
  ThreadArgs *args = (ThreadArgs *) opts;
  uint32_t *res = malloc(sizeof(uint32_t));
  *res = 0;
  uint32_t rand = 1212121;

  uint64_t i;
  for (i = 0; i < args->num_rands / args->num_threads; i++)
    *res += rand32(&rand);

  return res;
}

void *rand128_new(void *opts) {
  
  ThreadArgs *args = (ThreadArgs *) opts;
  uint32_t *res = malloc(sizeof(uint32_t));
  *res = 0;
  rand128_t *rand = new_rand128(1232313);

  uint64_t i;
  for (i = 0; i < args->num_rands / args->num_threads; i++)
    *res += rand128(rand);

  return res;
}

void *serial_rand_old(void *opts) {
  
  ThreadArgs *args = (ThreadArgs *) opts;
  uint32_t *res = malloc(sizeof(uint32_t));
  *res = 0;

  uint64_t i;
  for (i = 0; i < args->num_rands / args->num_threads; i++)
    *res += rand32_old();

  return res;
}

void *parallel_rand_old(void *opts) {
  
  ThreadArgs *args = (ThreadArgs *) opts;
  uint32_t *res = malloc(sizeof(uint32_t));
  *res = 0;

  uint64_t i;
  for (i = 0; i < args->num_rands / args->num_threads; i++)
    *res += rand32_old(args->i);

  return res;
}


void *normal_rand(void *opts) {
  
  ThreadArgs *args = (ThreadArgs *) opts;
  uint32_t *res = malloc(sizeof(uint32_t));
  *res = 0;

  uint64_t i;
  for (i = 0; i < args->num_rands / args->num_threads; i++)
    *res += rand();

  return res;
}

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("%s <num rands> <num threads>\n", argv[0]);
    exit(1);
  }

  uint64_t i, j, num_rands = atoi(argv[1]);
  uint64_t num_threads     = atoi(argv[2]);

  init_rand32_serial(time(NULL));
  init_rand32(num_threads);
  srand(time(NULL));


  TClient *clients  = malloc(sizeof(TClient)   * num_threads);
  ThreadArgs **args = malloc(sizeof(ThreadArgs *) * num_threads);

  for (i = 0; i < num_threads; i++) {
    init_thread_client(&clients[i]);

    args[i] = malloc(sizeof(ThreadArgs));
    args[i]->i = i;
    args[i]->num_threads = num_threads;
    args[i]->num_rands   = num_rands;
  }

  
  struct timeval rand32_r, rand128_r, serial_r, parallel_r, normal_r;// result
  struct timeval rand32_s, rand128_s, serial_s, parallel_s, normal_s;// start
  struct timeval rand32_e, rand128_e, serial_e, parallel_e, normal_e;// end


  gettimeofday(&rand32_s, NULL);
  for (j = 0; j < num_threads; j++)
    tc_add_func(&clients[j], rand32_new, (void *) args[j]);
  
  for (j = 0; j < num_threads; j++) 
    tc_join(&clients[j]);

  gettimeofday(&rand32_e, NULL);
  timeval_subtract(&rand32_r, &rand32_e, &rand32_s);

  gettimeofday(&rand128_s, NULL);
  for (j = 0; j < num_threads; j++)
    tc_add_func(&clients[j], rand128_new, (void *) args[j]);
  
  for (j = 0; j < num_threads; j++) 
    tc_join(&clients[j]);

  gettimeofday(&rand128_e, NULL);
  timeval_subtract(&rand128_r, &rand128_e, &rand128_s);

  gettimeofday(&serial_s, NULL);
  for (j = 0; j < num_threads; j++)
    tc_add_func(&clients[j], serial_rand_old, (void *) args[j]);
  
  for (j = 0; j < num_threads; j++) 
    tc_join(&clients[j]);

  gettimeofday(&serial_e, NULL);
  timeval_subtract(&serial_r, &serial_e, &serial_s);

  gettimeofday(&parallel_s, NULL);
  for (j = 0; j < num_threads; j++)
    tc_add_func(&clients[j], parallel_rand_old, (void *) args[j]);
  
  for (j = 0; j < num_threads; j++) 
    tc_join(&clients[j]);
  
  gettimeofday(&parallel_e, NULL);
  timeval_subtract(&parallel_r, &parallel_e, &parallel_s);

  gettimeofday(&normal_s, NULL);
  for (j = 0; j < num_threads; j++)
    tc_add_func(&clients[j], normal_rand, (void *) args[j]);
  
  for (j = 0; j < num_threads; j++) 
    tc_join(&clients[j]);
  
  gettimeofday(&normal_e, NULL);
  timeval_subtract(&normal_r, &normal_e, &normal_s);

  printf("rand32:   ");
  PRINT_TIMEVAL(rand32_r);
  printf("\n");
  
  printf("rand128:  ");
  PRINT_TIMEVAL(rand128_r);
  printf("\n");
  
  printf("parallel: ");
  PRINT_TIMEVAL(parallel_r);
  printf("\n");
  
  printf("serial:   ");
  PRINT_TIMEVAL(serial_r);
  printf("\n");

  printf("noarmal:  ");
  PRINT_TIMEVAL(normal_r);
  printf("\n");

  for (i = 0; i < num_threads; i++) {
    tc_free(&clients[i]);
    free(args[i]);
  }

  free(args);
  free(clients);

  return 0;
}
