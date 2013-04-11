#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/sort.h"
#include "../../Time-Diff/src/time-diff.h"

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

  if (argc != 5) {
    printf("%s <array length> <max min quicksort> "
           "<times> <threads>\n", argv[0]);
    exit(1);
  }

  srand(time(NULL) * rand());

  int64_t length = atoi(argv[1]);
  int64_t max_min_q = atoi(argv[2]);
  int64_t n_times = atoi(argv[3]);
  int64_t n_threads = atoi(argv[4]);
  struct timeval min_r, max_r, mmin_r, mmax_r, tmin_r, tmax_r; // result
  struct timeval min_t, max_t, mmin_t, mmax_t, tmin_t, tmax_t; // temp
  struct timeval min_s, max_s, mmin_s, mmax_s, tmin_s, tmax_s; // start
  struct timeval min_e, max_e, mmin_e, mmax_e, tmin_e, tmax_e; // end

  min_r.tv_sec  = min_r.tv_usec  = 0;
  max_r.tv_sec  = max_r.tv_usec  = 0;
  tmin_r.tv_sec = tmin_r.tv_usec = 0;
  tmax_r.tv_sec = tmax_r.tv_usec = 0;
  mmin_r.tv_sec = mmin_r.tv_usec = 0;
  mmax_r.tv_sec = mmax_r.tv_usec = 0;

  int64_t *ary1 = malloc(sizeof(int64_t) * length);

  int64_t i, k;

  int64_t sorted = 1;

  init_tc((uint32_t) log2((double) length), n_threads, 0);
  QISTA_t args;
  args.ary    = (void *) ary1;
  args.base   = sizeof(int64_t);
  args.length = length;
  args.min    = max_min_q;
  args.bigger = p_bigger;
  args.smaler = p_smaler;
  args.equal  = p_equal;

  for (k = 0; k < n_times; k++) {

    printf("times %" PRId64 "-6    \n", n_times - k);

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    gettimeofday(&min_s, NULL);
    quickinsersort_min(ary1, 
                       length, 
                       max_min_q, 
                       sizeof(int64_t), 
                       p_smaler, 
                       p_bigger, 
                       p_equal);
    gettimeofday(&min_e, NULL);
    timeval_subtract(&min_t, &min_e, &min_s);
    timeval_additon(&min_r, &min_r, &min_t);

    for (i = 0; i < length-1; i++) {
      if (ary1[i] > ary1[i+1])
        sorted = 0;
    }

    printf("times %" PRId64 "-5    \n", n_times - k);

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    gettimeofday(&mmin_s, NULL);
    QUICK_INSERT_SORT_MIN(int64_t,
                          ary1, 
                          length, 
                          bigger, 
                          smaler, 
                          equal, 
                          max_min_q);
    gettimeofday(&mmin_e, NULL);
    timeval_subtract(&mmin_t, &mmin_e, &mmin_s);
    timeval_additon(&mmin_r, &mmin_r, &mmin_t);

    for (i = 0; i < length-1; i++) {
      if (ary1[i] > ary1[i+1])
        sorted = 0;
    }

    printf("times %" PRId64 "-4    \n", n_times - k);

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    gettimeofday(&tmin_s, NULL);
    paralell_quickinsersort_min(&args);
    gettimeofday(&tmin_e, NULL);
    timeval_subtract(&tmin_t, &tmin_e, &tmin_s);
    timeval_additon(&tmin_r, &tmin_r, &tmin_t);

    for (i = 0; i < length-1; i++) {
      if (ary1[i] > ary1[i+1])
        sorted = 0;
    }

    printf("times %" PRId64 "-3    \n", n_times - k);

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    gettimeofday(&max_s, NULL);
    quickinsersort_max(ary1, 
                       length, 
                       max_min_q, 
                       sizeof(int64_t), 
                       p_smaler, 
                       p_bigger, 
                       p_equal);
    gettimeofday(&max_e, NULL);
    timeval_subtract(&max_t, &max_e, &max_s);
    timeval_additon(&max_r, &max_r, &max_t);

    for (i = 0; i < length-1; i++) {
      if (ary1[i] < ary1[i+1])
        sorted = 0;
    }

    printf("times %" PRId64 "-2    \n", n_times - k);
    
    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    gettimeofday(&mmax_s, NULL);
    QUICK_INSERT_SORT_MAX(int64_t,
                          ary1, 
                          length, 
                          bigger, 
                          smaler, 
                          equal, 
                          max_min_q);
    gettimeofday(&mmax_e, NULL);
    timeval_subtract(&mmax_t, &mmax_e, &mmax_s);
    timeval_additon(&mmax_r, &mmax_r, &mmax_t);

    for (i = 0; i < length-1; i++) {
      if (ary1[i] < ary1[i+1])
        sorted = 0;
    }

    printf("times %" PRId64 "-1    \n", n_times - k);

    for (i = 0; i < length; i++) {
      ary1[i] = rand() % length;
    }

    gettimeofday(&tmax_s, NULL);
    paralell_quickinsersort_max(&args);
    gettimeofday(&tmax_e, NULL);
    timeval_subtract(&tmax_t, &tmax_e, &tmax_s);
    timeval_additon(&tmax_r, &tmax_r, &tmax_t);

    for (i = 0; i < length-1; i++) {
      if (ary1[i] < ary1[i+1])
        sorted = 0;
    }

    printf("times %" PRId64 "-0    \n", n_times - k);

  }

  if (sorted == 1)
    printf("Everything went fine, arrays are sorted :-)\n\n");
  else
    printf("Array NOT sorted!!!\n");

  timeval_division(&min_r, &min_r, n_times);
  timeval_division(&max_r, &max_r, n_times);
  timeval_division(&mmin_r, &mmin_r, n_times);
  timeval_division(&mmax_r, &mmax_r, n_times);
  timeval_division(&tmin_r, &tmin_r, n_times);
  timeval_division(&tmax_r, &tmax_r, n_times);

  printf("quickinsertsort_min: ");
  PRINT_TIMEVAL(min_r);
  printf("\n");
  
  printf("quickinsertsort_max: ");
  PRINT_TIMEVAL(max_r);
  printf("\n");
  
  printf("QUICK_INSERT_SORT_MIN: ");
  PRINT_TIMEVAL(mmin_r);
  printf("\n");
  
  printf("QUICK_INSERT_SORT_MAX: ");
  PRINT_TIMEVAL(mmax_r);
  printf("\n");

  printf("paralell_quickinsersort_min: ");
  PRINT_TIMEVAL(tmin_r);
  printf("\n");
  
  printf("paralell_quickinsersort_max: ");
  PRINT_TIMEVAL(tmax_r);
  printf("\n");


  return 0;
}
    
