/**
 * a Thread Client is one running kerne level thread with can
 * progress any threadable function you want to process
 */
#ifndef __THREAD_CLIENT_H__
#define __THREAD_CLIENT_H__

/**
 * Thread-Client struct containing inforamtion of one thread client
 */
typedef struct {
  pthread_t thread;             /* the thread of this client                 */
  char      running;            /* indicates if thread client is running     */
  char      working;            /* indicates if thread is/should work(ing)   */
  void      *(*func) (void *);  /* function to process                       */
  void      *args               /* function parameter                        */
  void      *ret                /* function return value                     */
} TClient;

/**
 * creats an new Thread-Client;
 */
TClient new_thread_client();

/**
 * adds a function to process
 * Note: this onely works if the given client is nor working
 */
char tc_add_func(TClient *tc, void *(*func)(void *), void *args);

/**
 * Yields the processor until the given client has finished work
 * returns the return value of the processed function
 */
void *tc_join(TClient *tc);

/**
 * waits untill the current work is done an then stops and frees given client
 */
void tc_free(TClient *tc);

/**
 * kills running client and frees it
 */
tc_kill(TClient *tc);

#endif /* __THREAD_CLIENT_H__ */
