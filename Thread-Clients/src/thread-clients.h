#ifndef __THREAD_CLIENTS_H__
#define __THREAD_CLIENTS_H__

#include <inttypes.h>
#include <pthread.h>
#include "../../Dynamic-Array/src/dynamic-array.h"

/**
 * Arguments given to the processed functions
 * containing:
 *  - a thread id 0,1,..,n
 *  - client specific args (can be changed any time)
 *    these arges are given to each function processed
 *    by the same client.
 *    usefull for reusing memory by functions processed 
 *    by the same thread (avoids expensive malloc syscall)
 *  - function specific args (given by adding an function)
 */
typedef struct {
  const int  client_id;
  void       *client_args;
  void       *func_args;
} TCArgs;

/**
 * Initializes Thread-Clients Lib
 *
 * Parameters:
 *    max_work:   the expected maximum among of
 *                function given at a time to process
 *    n_clients:  the number of clients (threads) to use
 *    use_return: if you need the return values this sould be 1 (true)
 *                else let it be 0 and we save memory
 *                NOTE: if you set use_return and dosent pull the
 *                      return values memory will soooon be full
 */
void init_tc(uint32_t max_work, uint32_t n_clients);

/**
 * Sets the Opts of the specific client to the given pointer
 */
void tc_set_client_opts(uint32_t id, void *opts);

/**
 * Sets al clients Opts to the given void pointer array
 * first element will be assigned to the first client (id=0)
 */
void tc_set_clients_opts(void **opts);

/**
 * let you control wheter to use the return values or not
 * WARNING: if you shutdwon using return vallues all
 *          unsaved return values will be deleted
 *
 * give TRUE ore FALSE as parameter
 */
void tc_use_return(char use_return);

/**
 * adds a function to be proccesed paralell
 * you cann add as many functions (with their specific args)
 * as you want thei will pe progressed in random order by the
 * thread clients
 *
 * return:
 *    it returns the function id of the inserted function
 *    you will ned this id to get the return value of that function
 *
 *  Note: the your function will get an void pointer to a TCArgs
 *        struct conatining your given args
 *        WARNING: never try to free this pointer!!!
 */
uint64_t tc_add_func(void *(*func)(TCArgs *), void *args);

/**
 * Returns the return value of the function with the given id
 * or NULL, if there is no such function.
 *
 * NOTE: you should call tc_join() becor calling this
 *       else the return value mybe not yet avilable
 *
 * NOTE: if you set use_return and dosent pull the
 *       return values memory will soooon be full
 */
void *tc_get_return(uint64_t id);

/**
 * Clears all saved return pointers
 * NOTE: it doesnt frees anything
 *
 * (usefull if you have some NULL pointers)
 */
void tc_clear_returns();

/**
 * waiting utill all current work is done
 * (all functions are progressed)
 */
void tc_join();

/**
 * stops all clients and destroys waiting and finished work
 */
void tc_destroy();

#endif // __THREAD_CLIENTS_H__
