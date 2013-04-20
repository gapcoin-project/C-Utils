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
  char      do_work;            /* indicates if thread is/should work(ing)   */
  void      *(*func) (void *);  /* function to process                       */
  void      *args;              /* function parameter                        */
  void      *ret;               /* function return value                     */
} TClient;

/**
 * inits a given Thread-Client;
 */
void init_thread_client(TClient *);

/**
 * adds a function to process
 * Note: this onely works if the given client is nor working
 */
char tc_add_func(TClient *, void *(*)(void *), void *);

/**
 * Yields the processor until the given client has finished work
 * returns the return value of the processed function
 */
void *tc_join(TClient *);

/**
 * waits untill the current work is done an then stops and frees given client
 */
void tc_free(TClient *);

/**
 * kills running client and frees it
 */
void tc_kill(TClient *);

#endif /* __THREAD_CLIENT_H__ */
