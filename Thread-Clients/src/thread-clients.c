/**
 * Implementation of Thread Clients
 * a client is one threath woh is waiting for a function to progress
 * an then again waits untill the next function wants to be progressed
 */
#ifndef __THREAD_CLIENTS__
#define __THREAD_CLIENTS__

#include "thread-clients.h"
#include "../../Debug/src/debug.h"

/**
 * client function which processes work and then 
 * yields proccessor until new work is threre
 *
 * args will be the TClients struct
 */
static void *tcs_client_func(void *arg);

/**
 * indicates wether Thread-Clients are initialized or not
 */
static char tcs_initialized = 0;

/**
 * Struct holding one function and her args
 * (function must return an void pointer,
 *  and gehts an void pointer (args))
 */
typedef struct {
  void     *(*func) (TCArgs *);
  void     *args;
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
 pthread_t    *clients;   /* the client threads                        */
 TCArgs       *args;      /* client wide args                          */
 TCFuncsAry   work;       /* the work for the clients                  */
 TCFuncsAry   finished;   /* finished functions and their return value */
 uint8_t      running;    /* while true clients keep running           */
 uint8_t      use_return; /* wheter to use the return values or not    */
 uint64_t     last_id;    /* last func id                              */
 uint32_t     n_clients;  /* number of thread clients                  */
 uint32_t     n_working;  /* number of working threads                 */
} TClients;

/* to share values between clients */
static TClients tcs_clients;

/* to avoid race conditions */
static pthread_mutex_t tcs_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Args for the client function
 * containing an id and an pointer 
 * to the tcs_clients value
 */
typedef struct {
  int id;
  TClients *tcs_clients;
} TCClientsArgs;

/**
 * Initializes Thread-Clients Lib
 *
 * Parameters:
 *    max_work:   the expected maximum among of
 *                function given at a time to process
 *    n_clients:  the number of clients (threads) to use
 *
 *    Note: at default return values are not stored, if you need them
 *          set it with tcs_use_return(1);
 */
void init_tc(uint32_t max_work, uint32_t n_clients) {

  if (tcs_initialized) {
    DBG_MSG("Thread Clients allready initialized");
    return;
  }

  tcs_initialized = 1;

  ARY_INIT(TCFunc, tcs_clients.work, max_work);
  tcs_clients.clients    = malloc(sizeof(pthread_t) * n_clients);
  tcs_clients.args       = malloc(sizeof(TCArgs) * n_clients);
  tcs_clients.running    = 1;
  tcs_clients.use_return = 0;
  tcs_clients.last_id    = 1;
  tcs_clients.n_clients  = n_clients;
  tcs_clients.n_working  = 0;


  /* create client threads */

  uint32_t i;
  for (i = 0; i < n_clients; i++) {
    
    *((int *) &tcs_clients.args[i].client_id) = i;
    tcs_clients.args[i].client_args           = NULL;
    tcs_clients.args[i].func_args             = NULL;

    TCClientsArgs *tcs_args = malloc(sizeof(TCClientsArgs));
    tcs_args->id             = i;
    tcs_args->tcs_clients     = &tcs_clients;

    pthread_create(tcs_clients.clients + i, 
                   NULL, 
                   tcs_client_func, 
                   (void *) tcs_args);
  }
}

/**
 * Sets the Opts of the specific client to the given pointer
 */
void tcs_set_client_opts(uint32_t id, void *opts) {
  if (id >= tcs_clients.n_clients) {
    WARN_MSG("no Client with the given id");
    return;
  }

  tcs_clients.args[id].client_args = opts;
}

/**
 * Sets al clients Opts to the given void pointer array
 * first element will be assigned to the first client (id=0)
 */
void tcs_set_clients_opts(void **opts) {

  uint32_t i;
  for (i = 0; i < tcs_clients.n_clients; i++)
    tcs_clients.args[i].client_args = opts[i];

}

/**
 * let you control wheter to use the return values or not
 * WARNING: if you shutdwon using return vallues all
 *          unsaved return values will be deleted
 *
 * give TRUE ore FALSE as parameter
 *
 * NOTE: if you set use_return and dosent pull the
 *       return values memory will soooon be full
 */
void tcs_use_return(char use_return) {

  if (use_return && !tcs_clients.use_return)
    ARY_INIT(TCFunc, tcs_clients.finished, ARY_LEN(tcs_clients.work));
  else
    ARY_FREE(tcs_clients.finished);

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
uint64_t tcs_add_func(void *(*func)(TCArgs *), void *args) {
  
  TCFunc new_func;

  new_func.func = func;
  new_func.args = args;
  new_func.id   = ++tcs_clients.last_id;
  
  
  /* avoid race conditions */
  pthread_mutex_lock(&tcs_mutex);

  ARY_ADD(tcs_clients.work, new_func);

  pthread_mutex_unlock(&tcs_mutex);

  return new_func.id;
}

/**
 * Returns the return value of the function with the given id
 * or NULL, if there is no such function.
 *
 * NOTE: you should call tcs_join() becor calling this
 *       else the return value mybe not yet avilable
 */
void *tcs_get_return(uint64_t id) {

  uint64_t i;
  for (i = 0; i < ARY_LEN(tcs_clients.finished); i++) {
    if (ARY_AT(tcs_clients.finished, i).id == id) {

      void *return_val = ARY_AT(tcs_clients.finished, i).args;
      ARY_DELETE_AT_FAST(tcs_clients.finished, i);

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
void tcs_clear_returns() {
  ARY_CLEAR(tcs_clients.finished);
}


/**
 * waiting utill all current work is done
 * (all functions are progressed)
 */
void tcs_join() {

  while (tcs_clients.n_working || ARY_LEN(tcs_clients.work) > 0) 
    sched_yield();

}

/**
 * stops all clients and destroys waiting and finished work
 */
void tcs_destroy() {

  tcs_clients.running = 0;

  /* waiting for clients to finish */
  uint32_t i;
  for (i = 0; i < tcs_clients.n_clients; i++)
    pthread_join(tcs_clients.clients[i], NULL);

  ARY_FREE(tcs_clients.work);
  if (tcs_clients.use_return)
    ARY_FREE(tcs_clients.finished);

  free(tcs_clients.clients);

}

/**
 * client function which processes work and then 
 * yields proccessor until new work is threre
 *
 * args will be the TClients struct
 */
static void *tcs_client_func(void *arg) {

  /** 
   * TClients struct containig the work todo 
   * is is neccesary to give it as the parameter,
   * because the function pointer hase no direct access to
   * the tcs_clients static struct which is in this file
   * */
  TClients *tcs_clients = ((TCClientsArgs *) arg)->tcs_clients;
  int client_id        = ((TCClientsArgs *) arg)->id;
  free(arg);

  /* will contain the next function (work) to progress */
  TCFunc next_work;
  
  /* working loop (no mutex needed) */
  while (tcs_clients->running) {
    
    /* pull next work funcion savely */
    pthread_mutex_lock(&tcs_mutex);
    
    if (ARY_LEN(tcs_clients->work) > 0) {
      
      tcs_clients->n_working++;
      ARY_EXTRACT(tcs_clients->work, next_work); /* get next work */
      pthread_mutex_unlock(&tcs_mutex); /* call work paralell */

      /* set args */
      tcs_clients->args[client_id].func_args = next_work.args;

      /* start working */
      next_work.args = next_work.func(&tcs_clients->args[client_id]);

      pthread_mutex_lock(&tcs_mutex);
      tcs_clients->n_working--;
      pthread_mutex_unlock(&tcs_mutex);

      /* save return if wanted */
      if (tcs_clients->use_return) {
        pthread_mutex_lock(&tcs_mutex);

        ARY_PUSH(tcs_clients->finished, next_work);

        pthread_mutex_unlock(&tcs_mutex);
      }
      
    } else { /* nothing to do release cpu */
      pthread_mutex_unlock(&tcs_mutex);
      sched_yield();
    }

  }

  return NULL;
}




#endif // __THREAD_CLIENTS__
