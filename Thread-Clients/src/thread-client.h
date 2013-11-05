/**
 * a Thread Client is one running kerne level thread with can
 * progress any threadable function you want to process
 */
#ifndef __THREAD_CLIENT_H__
#define __THREAD_CLIENT_H__
#include <pthread.h>

/**
 * Thread-Client struct containing inforamtion of one thread client
 */
typedef struct {
  pthread_t thread;             /* the thread of this client                 */
  char      running;            /* indicates if thread client is running     */
  void      *(*old) (void *);   /* function to process                       */
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
 * sets the scheduling policy and priority of the tc thread
 */
static inline void tc_set_scheduler(TClient *tc, int policy, int priority) {
  
  struct sched_param param;
  param.sched_priority = priority;

  pthread_setschedparam(tc->thread, policy, &param);
}

/**
 * returns the scheduling policy adn priority of the tc thread
 */
static inline void tc_get_scheduler(TClient *tc, int *policy, int *priority) {

  struct sched_param param;
  pthread_getschedparam(tc->thread, policy, &param);

  *priority = param.sched_priority;
}

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
 *       (returns true if function coud be added else false)
 */
static inline char tc_add_func(TClient *tc, 
                               void *(*func)(void *), 
                               void *args) {

  if (tc->func != NULL)
    return 0;

  tc->func    = func;
  tc->old     = func;
  tc->args    = args;

  pthread_mutex_unlock(&tc->add);

  return 1;
}


/**
 * adds a function to process, but dont start to progressing it,
 * tc_run hav to be called for that
 *
 * Note: it failes if a previous function hasend finished
 *       (returns true if function coud be added else false)
 */
static inline char tc_only_add_func(TClient *tc, 
                                    void *(*func)(void *), 
                                    void *args) {

  if (tc->func != NULL)
    return 0;

  tc->func = func;
  tc->old  = func;
  tc->args = args;

  return 1;
}

/**
 * starts progressing an tc function if only_add had be unsed
 * Note: the behavior is unexpected if normal add function was used
 */
#define tc_run(tc) pthread_mutex_unlock(&(tc)->add)

/**
 * Sets only the old function and the args, so you can call rerun afterwards 
 * (int doesnt start computing, you have to call rerun to start it)
 *
 * Note: it failes if a previous function hasend finished
 *       (returns true if function coud be added else false)
 */
static inline char tc_set_rerun_func(TClient *tc, 
                                     void *(*func)(void *), 
                                     void *args) {

  if (tc->func != NULL)
    return 0;

  tc->old  = func;
  tc->args = args;

  return 1;
}

/**
 * reruns the last added function, with the last added args
 * Note: it failes if a previous function hasend finished
 *       (returns true if function coud be added else false)
 */
static inline char tc_rerun(TClient *tc) {

  if (tc->func != NULL)
    return 0;

  tc->func = tc->old;

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
