/**
 * a Thread Client is one running kerne level thread with can
 * progress any threadable function you want to process
 */
#ifndef __THREAD_CLIENT__
#define __THREAD_CLIENT__

#include "thread-client.h"
#include "../../Debug/src/debug.h"
#include <pthread.h>

/* static function */
static void *tc_client_func(void *arg);

/**
 * creats an new Thread-Client;
 */
TClient new_thread_client() {
  
  TClient *tc = malloc(sizeof(TClient);
  
  tc->running = 1;
  tc->do_work = 0;
  tc->func    = NULL;
  tc->args    = NULL;

  pthread_create(&tc->thread, NULL, tc_client_func, (void *) tc);

  return tc;

}

/**
 * adds a function to process
 * Note: this onely works if the given client is nor do_work
 */
char tc_add_func(TClient *tc, void *(*func)(void *), void *args) {

  if (tc->do_work) {
    DBG_MSG("failed to add func TC is do_work");
    return 0;
  }

  tc->func    = func;
  tc->args    = args;
  tc->do_work = 1;

}

/**
 * Yields the processor until the given client has finished work
 * returns the return value of the processed function
 */
void *tc_join(TClient *tc) {
  
  while (tc->do_work)
    sched_yield();

  return tc->ret;

}

/**
 * waits untill the current work is done an then stops and frees given client
 */
void tc_free(TClient *tc) {

  tc->running = 0;
  pthread_join(tc->thread, NULL);
  free(tc);

}

/**
 * kills running client and frees it
 */
tc_kill(TClient *tc) {

  pthread_cancel(tc->thread);
  free(tc);

}

/**
 * The client function wich procresses the functions (work)
 * and yields th processor until new work is there
 */
static void *tc_client_func(void *arg) {
  
  TClient *tc = (TClient *) arg;

  while (tc->running) {
    
    if (tc->do_work) {

      tc->ret = tc->func(tc->args);
      tc->do_work = 0;

    } else
      while (!tc->do_work && tc->running)
        sched_yield();

  }

  return NULL;

}



#endif /* __THREAD_CLIENT__ */
