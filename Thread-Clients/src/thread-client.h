/**
 * a Thread Client is one running kerne level thread with can
 * progress any threadable function you want to process
 */
#ifndef __THREAD_CLIENT_H__
#define __THREAD_CLIENT_H__
#include <pthread.h>
#include <semaphore.h>

/**
 * Thread-Client struct containing inforamtion of one thread client
 */
typedef struct {
  pthread_t thread;             /* the thread of this client                 */
  char      running;            /* indicates if thread client is running     */
  void      *(*func) (void *);  /* function to process                       */
  void      *args;              /* function parameter                        */
  void      *ret;               /* function return value                     */
  pthread_mutex_t add;          /* lock for adding a function                */
  pthread_mutex_t run;          /* lock for sync run                         */
  pthread_mutex_t join;         /* lock for implement join                   */
} TClient;

/**
 * inits a given Thread-Client;
 */
void init_thread_client(TClient *);

/**
 * Frees a given TClient
 * Note: you shuld have called join bevore this
 *       else the behavior is unexpected!
 */
void tc_free(TClient *);

/**
 * kills running client and frees it
 */
void tc_kill(TClient *);

/**
 * adds a function to process
 * Note: it failes if a previous function hasend finished
 *       (returns true if function cluld be added else false)
 */
static inline char tc_add_func(TClient *tc, 
                               void *(*func)(void *), 
                               void *args) {

  if (tc->func != NULL)
    return 0;

  tc->func    = func;
  tc->args    = args;

  pthread_mutex_unlock(&tc->add);

  return 1;

}

/**
 * waits untill a function has finisehd 
 * Note: a function has to be joined to run the next function
 */
static inline void *tc_join(TClient *tc) {
  
  /* waits untill a function has finisehd */
  pthread_mutex_lock(&tc->join);
  pthread_mutex_unlock(&tc->run);

  return tc->ret;

}


#endif /* __THREAD_CLIENT_H__ */
