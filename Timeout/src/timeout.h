/**
 *  Implementaion of an fork, shared memory and sigterm based timout
 */
#ifndef __TIMEOUT_H__
#define __TIMEOUT_H__

#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <linux/sched.h>

/**
 * global variable to determinate whether a function was 
 * shutdown by timout or not
 */
static int __was_timouted__ = 0;

/**
 * Process an function wih an agressive Timout
 * the given function will be runn in a new thread
 * sharing the same address space and file pointers.
 *
 * The timout is processed du to an SIGALARM
 * NOTE: this is my be very dangerous because it interuppts 
 * the called function within the same address space and so 
 * my some library static buffes have invalid values and behave unexepected
 */
#define WITH_TIMEOUT_DO(USEC, FUNC, ...)                                      \
do {                                                                          \
                                                                              \
  /* timer value */                                                           \
  struct itimerval __timer_val__ = { { 0,  0 },                               \
                                   { (USEC) / 1000000, (USEC) % 1000000 } };  \
                                                                              \
  pid_t __pid__ = vfork();                                                    \
                                                                              \
  if (__pid__ < 0) {                                                          \
    perror("vfork failed in WITH_TIMEOUT_DO");                                \
    exit(EXIT_FAILURE);                                                       \
                                                                              \
  } else if (__pid__ > 0) {                                                   \
    waitpid(__pid__, &__was_timouted__, 0);                                   \
                                                                              \
  } else {                                                                    \
                                                                              \
    /* reset timer siganl handler */                                          \
    signal(SIGALRM, SIG_DFL);                                                 \
                                                                              \
    /* start timer */                                                         \
    setitimer(ITIMER_REAL, &__timer_val__, NULL);                             \
                                                                              \
    /* runn function */                                                       \
    FUNC(__VA_ARGS__);                                                        \
                                                                              \
    /* use agresive exit to not destroy the parents stack */                  \
    _exit(EXIT_SUCCESS);                                                      \
  }                                                                           \
} while (0)

/**
 * returns wheter the last timout-function was timouted
 */
#define TIMEOUT_REATCHED                                                    \
  (WIFSIGNALED(__was_timouted__) && WTERMSIG(__was_timouted__) == SIGALRM)

/**
 * Process an function wih an agressive Timout
 * the given funktion pointer will pe brocessed in
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
