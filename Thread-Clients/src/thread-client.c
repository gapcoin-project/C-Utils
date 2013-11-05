/**
 * a Thread Client is one running kerne level thread with can
 * progress any threadable function you want to process
 */
#ifndef __THREAD_CLIENT__
#define __THREAD_CLIENT__

#include "thread-client.h"
#include "../../Debug/src/debug.h"

/* static function */
static void *tc_client_func(void *);

/**
 * inits a given Thread-Client;
 */
void init_thread_client(TClient *tc) {
  
  tc->running = 1;
  tc->func    = NULL;
  tc->args    = NULL;
  pthread_mutex_init(&tc->add, NULL);
  pthread_mutex_init(&tc->run, NULL);
  pthread_mutex_init(&tc->join, NULL);

  /* a function have to be added first */
  pthread_mutex_lock(&tc->join);
  pthread_mutex_lock(&tc->add);

  pthread_create(&tc->thread, NULL, tc_client_func, (void *) tc);

}

/**
 * Frees a given TClient
 * Note: you shuld have called join bevore this
 *       else the behavior is unexpected!
 */
void tc_free(TClient *tc) {

  tc->running = (char) 0;

  /* unlock mutexes to exit tc_client_func */
  pthread_mutex_trylock(&tc->add);
  pthread_mutex_unlock(&tc->add);

  pthread_mutex_trylock(&tc->run);
  pthread_mutex_unlock(&tc->run);

  /* wait for tc_client_func to become finished */
  pthread_join(tc->thread, NULL);

  /* unlock all mutexes */
  pthread_mutex_trylock(&tc->add);
  pthread_mutex_unlock(&tc->add);

  pthread_mutex_trylock(&tc->run);
  pthread_mutex_unlock(&tc->run);

  pthread_mutex_trylock(&tc->join);
  pthread_mutex_unlock(&tc->join);

  /* destroy the mutexes */
  pthread_mutex_destroy(&tc->add);
  pthread_mutex_destroy(&tc->run);
  pthread_mutex_destroy(&tc->join);
}

/**
 * kills running client and frees it
 */
void tc_kill(TClient *tc) {

  pthread_cancel(tc->thread);

  /* unlock all mutexes */
  pthread_mutex_trylock(&tc->add);
  pthread_mutex_unlock(&tc->add);

  pthread_mutex_trylock(&tc->run);
  pthread_mutex_unlock(&tc->run);

  pthread_mutex_trylock(&tc->join);
  pthread_mutex_unlock(&tc->join);

  /* destroy the mutexes */
  pthread_mutex_destroy(&tc->add);
  pthread_mutex_destroy(&tc->run);
  pthread_mutex_destroy(&tc->join);
}

/**
 * The client function wich procresses the functions (work)
 * and yields th processor until new work is there
 */
static void *tc_client_func(void *arg) {
  
  TClient *tc = (TClient *) arg;

  while (tc->running) {

    pthread_mutex_lock(&tc->add);
    pthread_mutex_lock(&tc->run);
    
    if (tc->running) {
      tc->ret  = tc->func(tc->args);
      tc->func = NULL;
    }

    pthread_mutex_unlock(&tc->join);
  }

  return NULL;
}



#endif /* __THREAD_CLIENT__ */
