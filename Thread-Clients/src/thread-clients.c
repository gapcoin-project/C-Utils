/**
 * Implementation of Thread Clients
 * a client is one threath woh is waiting for a function to progress
 * an then again waits untill the next function wants to be progressed
 */
#ifndef __THREAD_CLIENTS__
#define __THREAD_CLIENTS__

#include "thread-clients.h"

/**
 * client function which processes work and then 
 * yields proccessor until new work is threre
 *
 * args will be the TClients struct
 */
static void *tc_client_func(void *arg);

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
DEF_ARY(TCFunc, TCFuncsAry);

/**
 * Struc holding the thread client and the work array
 */
typedef struct {
 pthread_t   *clients;    /* the client threads */
 TCFuncsAry   work;       /* the work for the clients */
 TCFuncsAry   finished;   /* finished functions and their return value */
 uint8_t      running;    /* while true clients keep running */
 uint8_t      use_return; /* wheter to use the return values or not */
 uint64_t     last_id;    /* last func id */
 uint32_t     n_clients;  /* number of thread clients */
 uint32_t     n_working;  /* number of working threads */
} TClients;

/* to share values between clients */
static TClients tc_clients;

/* to avoid race conditions */
static pthread_mutex_t tc_mutex = PTHREAD_MUTEX_INITIALIZER;

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

  ARY_INIT(TCFunc, tc_clients.work, max_work);
  if (use_return)
  ARY_INIT(TCFunc, tc_clients.finished, max_work);
  tc_clients.clients    = malloc(sizeof(pthread_t) * n_clients);
  tc_clients.running    = 1;
  tc_clients.use_return = use_return;
  tc_clients.last_id    = 1;
  tc_clients.n_clients  = n_clients;
  tc_clients.n_working  = 0;

  /* create client threads */
  uint32_t i;
  for (i = 0; i < n_clients; i++)
    pthread_create(tc_clients.clients + i, 
                   NULL, 
                   tc_client_func, 
                   (void *) &tc_clients);
}

/**
 * adds a function to be proccesed paralell
 * you cann ad as many functions (with their specific args)
 * as you want thei will pe progressed in random order by the
 * thread clients
 *
 * return:
 *    it returns the function id of the inserted function
 *    you will ned this id to get the return value of that function
 */
uint64_t tc_add_func(void *(*func)(void *), void *args) {
  
  TCFunc new_func;

  new_func.func = func;
  new_func.args = args;
  new_func.id   = ++tc_clients.last_id;
  
  
  /* avoid race conditions */
  pthread_mutex_lock(&tc_mutex);

  ARY_ADD(tc_clients.work, new_func);

  pthread_mutex_unlock(&tc_mutex);

  return new_func.id;
}

/**
 * Returns the return value of the function with the given id
 * or NULL, if there is no such function.
 *
 * NOTE: you should call tc_join() becor calling this
 *       else the return value mybe not yet avilable
 */
void *tc_get_return(uint64_t id) {

  uint64_t i;
  for (i = 0; i < ARY_LEN(tc_clients.finished); i++) {
    if (ARY_AT(tc_clients.finished, i).id == id) {

      void *return_val = ARY_AT(tc_clients.finished, i).args;
      ARY_DELETE_AT_FAST(tc_clients.finished, i);

      return return_val;
    }
  }

  return NULL;
}

/**
 * Clears all saved return pointers
 * NOTE: it doesnt frees anything
 *
 * (usefull if you have some NULL pointers)
 */
void tc_clear_returns() {
  ARY_CLEAR(tc_clients.finished);
}


/**
 * waiting utill all current work is done
 * (all functions are progressed)
 */
void tc_join() {

  while (tc_clients.n_working || ARY_LEN(tc_clients.work) > 0) 
    sched_yield();

}

/**
 * stops all clients and destroys waiting and finished work
 */
void tc_destroy() {

  tc_clients.running = 0;

  /* waiting for clients to finish */
  uint32_t i;
  for (i = 0; i < tc_clients.n_clients; i++)
    pthread_join(tc_clients.clients[i], NULL);

  ARY_FREE(tc_clients.work);
  if (tc_clients.use_return)
    ARY_FREE(tc_clients.finished);

  free(tc_clients.clients);

}

/**
 * client function which processes work and then 
 * yields proccessor until new work is threre
 *
 * args will be the TClients struct
 */
static void *tc_client_func(void *arg) {

  /** 
   * TClients struct containig the work todo 
   * is is neccesary to give it as the parameter,
   * because the function pointer hase no direct access to
   * the tc_clients static struct which is in this file
   * */
  TClients *tc_clients = (TClients *) arg;

  /* will contain the next function (work) to progress */
  TCFunc next_work;

  
  /* working loop (no mutex needed) */
  while (tc_clients->running) {
    
    /* pull next work funcion savely */
    pthread_mutex_lock(&tc_mutex);
    
    if (ARY_LEN(tc_clients->work) > 0) {
      
      tc_clients->n_working++;
      ARY_EXTRACT(tc_clients->work, next_work); /* get next work */
      pthread_mutex_unlock(&tc_mutex); /* call work paralell */

      /* start working */
      next_work.args = next_work.func(next_work.args);

      pthread_mutex_lock(&tc_mutex);
      tc_clients->n_working--;
      pthread_mutex_unlock(&tc_mutex);

      /* save return if wanted */
      if (tc_clients->use_return) {
        pthread_mutex_lock(&tc_mutex);

        ARY_PUSH(tc_clients->finished, next_work);

        pthread_mutex_unlock(&tc_mutex);
      }
      
    } else { /* nothing to do release cpu */
      pthread_mutex_unlock(&tc_mutex);
      sched_yield();
    }

  }

  return NULL;
}




#endif // __THREAD_CLIENTS__
