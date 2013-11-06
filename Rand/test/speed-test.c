#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>


#include "../../Thread-Clients/src/thread-client.h"
#include "../../Time-Diff/src/time-diff.h"

/**
 * the seeds internal usage only
 * (thread seed and serial seed)
 */
static uint32_t **rand32_seeds;
static uint32_t rand32_serial_seed;

/**
 * sets the serial seed to the given value
 */
#define init_serial_rand32(seed) rand32_serial_seed = seed

/**
 * returns the next serial rand
 */
static inline uint32_t next_serial_rand32() {
  
  uint32_t hi, lo;

  lo = 16807 * (rand32_serial_seed & 0xFFFF);
  hi = 16807 * (rand32_serial_seed >> 16);

  lo += (hi & 0x7FFF) << 16;
  lo += hi >> 15;

  if (lo > 0x7FFFFFFF) lo -= 0x7FFFFFFF;

  return (rand32_serial_seed = lo);
}

/**
 * returns the next rand number for the given thread index
 */
static inline  uint32_t next_rand32(uint16_t index) {

  uint32_t hi, lo = 0;

  lo = 16807 * ((*(rand32_seeds[index])) & 0xFFFF);
  hi = 16807 * (*rand32_seeds[index] >> 16);

  lo += (hi & 0x7FFF) << 16;
  lo += hi >> 15;

  if (lo > 0x7FFFFFFF) lo -= 0x7FFFFFFF;

  return (*rand32_seeds[index] = lo);
}

/**
 * initialzes rand with a given number of threads
 * (also initializes the seed with the current time)
 */
static inline void init_rand32(uint16_t num_threads) {
  
  rand32_seeds = malloc(sizeof(uint32_t *) * num_threads);
  
  if (rand32_seeds == NULL) {
    perror("malloc failed in init_rand32");
    exit(1);
  }

  uint16_t i;
  for (i = 0; i < num_threads; i++) {
    rand32_seeds[i] = malloc(sizeof(uint32_t));

    if (rand32_seeds[i] == NULL) {
      perror("malloc failed in init_rand32");
      exit(1);
    }

    *rand32_seeds[i] = ((i == 0) ? time(NULL) : next_rand32(i - 1));
  }
}

/**
 * thread args
 */
typedef struct {
  uint16_t i, num_threads;
  uint64_t num_rands;
} ThreadArgs;

void *serial_rand(void *opts) {
  
  ThreadArgs *args = (ThreadArgs *) opts;
  uint32_t *res = malloc(sizeof(uint32_t));
  *res = 0;

  uint64_t i;
  for (i = 0; i < args->num_rands / args->num_threads; i++)
    *res += next_serial_rand32();

  return res;
}

void *parallel_rand(void *opts) {
  
  ThreadArgs *args = (ThreadArgs *) opts;
  uint32_t *res = malloc(sizeof(uint32_t));
  *res = 0;

  uint64_t i;
  for (i = 0; i < args->num_rands / args->num_threads; i++)
    *res += next_rand32(args->i);

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

  init_serial_rand32(time(NULL));
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

  
  struct timeval serial_r, parallel_r, normal_r;// result
  struct timeval serial_s, parallel_s, normal_s;// start
  struct timeval serial_e, parallel_e, normal_e;// end


  gettimeofday(&serial_s, NULL);
  for (j = 0; j < num_threads; j++)
    tc_add_func(&clients[j], serial_rand, (void *) args[j]);
  
  for (j = 0; j < num_threads; j++) 
    tc_join(&clients[j]);

  gettimeofday(&serial_e, NULL);
  timeval_subtract(&serial_r, &serial_e, &serial_s);

  gettimeofday(&parallel_s, NULL);
  for (j = 0; j < num_threads; j++)
    tc_add_func(&clients[j], parallel_rand, (void *) args[j]);
  
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
