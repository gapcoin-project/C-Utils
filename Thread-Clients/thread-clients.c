/**
 * Implementation of Thread Clients
 * a client is one threath woh is waiting for a function to progress
 * an then again waits untill the next function wants to be progressed
 */
#ifndef __THREAD_CLIENTS__
#define __THREAD_CLIENTS__

#include "thread-clients.h"

/**
 * Struct holding one function and her args
 * (function must return an void pointer,
 *  and gehts an void pointer (args))
 */
typedef struct {
  void *(*func) (void *);
  void *args;
  uint64_t id;
} TCFunc;

/**
 * Dynamic Array of TCFuncs
 */
DEF_ARY(TCFuncs, TCFuncsAry);

/**
 * Struc holding the thread client and the work array
 */
typedef struct {
 pthread_t *clients;    /* the client threads */
 pthread_mutex_t mutex; /* mutex object to avoid race conditions */
 TCFuncsAry work;       /* the work for the clients */
 TCFuncsAry finished;   /* finished functions and their return value */
 uint8_t running;       /* while true clients keep running */
 uint8_t use_return;    /* wheter to use the return values or not */
} TClients;


/**
 * Initializes Thread-Clients Lib
 *
 * Parameters:
 *    max_work:   the expected maximum among of
 *                function given at a time to process
 *    n_clients:  the number of clients (threads) to use
 *    use_return: if you need the return values this sould be 1 (true)
 *                else let it be 0 and, save we save memory
 *                NOTE: if you set use_return and dosent pull the
 *                      return values memory will soooon be full
 */
void init_tc(uint32_t max_work, uint32_t n_clients, uint8_t use_return) {
  TClients tc_clients;

  ARY_INIT(TCFuncs, tc_clients.work, max_work);
  if (use_return)
  ARY_INIT(TCFuncs, tc_clients.finished, max_work);
  tc_clients.clients    = malloc(sizeof(pthread_t) * n_clients);
  tc_clients.mutex      = PTHREAD_MUTEX_INITIALIZER;
  tc_clients.running    =  1;
  tc_clients.use_return = use_return;
}

/**
 * client function which processes work and then 
 * yields proccessor until new work is threre
 *
 * args will be the TClients struct
 */
static void *tc_client_func(void *arg) {

  /* TClients struct containig the work todo */
  TClients *tc_clients = (TClients *) arg;

  /* will contain the next function (work) to progress */
  TCFunc next_work;

  
  /* working loop (no mutex needed) */
  while (tc_clients->running) {
    
    /* pull next work funcion savely */
    pthread_mutex_lock(&tc_clients->mutex);
    
    if (ARY_LEN(tc_clients->work) > 0) {
      
      ARY_PULL(tc_clients->work, next_work); /* get next ework */
      pthread_mutex_unlock(&tc_clients->mutex); /* call work paralell */

      /* start working */
      next_work.args = next_work.func(next_work.args);

      /* save return if wanted */
      if (tc_clients.use_return) {
        pthread_mutex_lock(&tc_clients->mutex);

        ARY_PUSH(tc_clients.finished, next_work);

        pthread_mutex_unlock(&tc_clients->mutex);
      }
      
    } else { /* nothing to do release cpu */
      pthread_mutex_unlock(&tc_clients->mutex);
      sched_yield();
    }

  }

  return NULL;
}




#endif // __THREAD_CLIENTS__
