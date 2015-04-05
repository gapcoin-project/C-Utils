#ifndef __TIME_DIFF_H__
#define __TIME_DIFF_H__

#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>

#define PRINT_TIMEVAL(TVAL)                                 \
  printf("%ld.%06ld", TVAL.tv_sec, TVAL.tv_usec);

/**
 * Return 1 if the difference is negative, otherwise 0.  
 * Saves the Difference of t1 and t2 in result
 */
char timeval_subtract(struct timeval *result, 
                      struct timeval *t2, 
                      struct timeval *t1);

/**
 * Return 1 if the addition is negative, otherwise 0.  
 * Saves the Addition of t1 and t2 in result
 */
char timeval_additon(struct timeval *result, 
                     struct timeval *t2, 
                     struct timeval *t1);

/**
 * Returns 1 if the Division is negative, otherwise 0.
 * Saves the Division of t1 by divisor in result
 */
char timeval_division(struct timeval *result, 
                      struct timeval *t1,
                      int divisor);

/**
 * Prints a Timevall struct
 */
void timeval_print(struct timeval *tv);

/**
 * returns the current time in microseconds
 */
uint64_t gettime_usec();

#endif // __TIME_DIFF_H__
