/**
 *  Implementaion of an fork, shared memory and sigterm based timout
 */
#ifndef __TIMEOUT_H__
#define __TIMEOUT_H__

#include <sys/shm.h>
#include <sys/shm.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * returns wether a setted soft timout was reatched
 */
char soft_timouted() { return soft_timout_reatched; }

/**
 * sets a soft timout using an sigalarm handler 
 * and a static value
 * negativ timout let the timout never expire
 */
void set_soft_timout(int64_t usec) {

/**
 * Process an function wih an agressive Timout
 * th given funktion pointer will pe brocessed in
 * a seperate process so if it will be canceld 
 * by timout the kernle frees unfreed memory
 *
 * time is the time to process func
 * func is an function pointer which gets
 *      a pointer to a shared memory buffer
 *      and a pointer to own args
 * shm_size is the size that buffer should have
 * args ar given as second parameter to func
 *
 * it returns the pointer to the shared memory
 *
 * Note: any changes on process memory in func
 *       will be discard so use the shared memory
 *       for return values and other memory changes
 */
void *with_timeout_do(struct timeval *time,
                      void (*func)(void *, void *),
                      size_t shm_size, void *args);

#endif /* __TIMEOUT_H__ */
