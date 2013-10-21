#ifndef __TIME_DIFF__
#define __TIME_DIFF__

#include "time-diff.h"

/**
 * Return 1 if the difference is negative, otherwise 0.  
 * Saves the Difference of t1 and t2 in result
 */
char timeval_subtract(struct timeval *result, 
                      struct timeval *t2, 
                      struct timeval *t1) {

    int64_t diff = (t2->tv_usec + 1000000L * t2->tv_sec) -
                   (t1->tv_usec + 1000000L * t1->tv_sec);

    result->tv_sec  = diff / 1000000L;
    result->tv_usec = diff % 1000000L;

    return (diff < 0);
}

/**
 * Return 1 if the addition is negative, otherwise 0.  
 * Saves the Addition of t1 and t2 in result
 */
char timeval_additon(struct timeval *result, 
                     struct timeval *t2, 
                     struct timeval *t1) {

    int64_t diff =  (t2->tv_usec + 1000000L * t2->tv_sec) +
                    (t1->tv_usec + 1000000L * t1->tv_sec);

    result->tv_sec  = diff / 1000000L;
    result->tv_usec = diff % 1000000L;

    return (diff < 0);
}

/**
 * Returns 1 if the Division is negative, otherwise 0.
 * Saves the Division of t1 by divisor in result
 */
char timeval_division(struct timeval *result, 
                      struct timeval *t1,
                      int divisor) {

    int64_t diff = (t1->tv_usec + 1000000L * t1->tv_sec) / divisor;

    result->tv_sec  = diff / 1000000L;
    result->tv_usec = diff % 1000000L;

    return (diff < 0);
}

/**
 * Prints a Timevall struct
 */
void timeval_print(struct timeval *tv) {

    char buffer[30];
    time_t curtime;

    printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
    curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    printf(" = %s.%06ld\n", buffer, tv->tv_usec);
}

#endif // __TIME_DIFF__
