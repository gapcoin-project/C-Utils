#ifndef __TIME_DIFF_H__
#define __TIME_DIFF_H__

#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * Return 1 if the difference is negative, otherwise 0.  
 * Saves the Difference of t1 and t2 in result
 */
int timeval_subtract(struct timeval *result, 
                     struct timeval *t2, 
                     struct timeval *t1);


/**
 * Prints a Timevall struct
 */
void timeval_print(struct timeval *tv);

#endif // __TIME_DIFF_H__
