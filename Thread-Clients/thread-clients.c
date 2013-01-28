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
 TCFuncsAry work;       /* the work for the clients */
} TClients;
static TClients tc_clients;

/**
 * Mutex to avoid race conditions
 */
static pthread_mutex_t tc_mutex = PTHREAD_MUTEX_INITIALIZER;


/**
 * Initializes Thread-Clients Lib
 *
 * Parameters:
 *    max_work:   the expected maximum among of
 *                function given at a time to process
 *    n_clients:  the number of clients (threads) to use
 */
void init_tc(uint32_t max_work, uint32_t n_clients) {
  ARY_INIT(TCFuncs, tc_clients.work, max_work);
  tc_clients.clients = malloc(sizeof(pthread_t) * n_clients);
}



#endif // __THREAD_CLIENTS__
