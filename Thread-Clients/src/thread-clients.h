#ifndef __THREAD_CLIENTS_H__
#define __THREAD_CLIENTS_H__

#include <inttypes.h>
#include <pthread.h>
#include "../../Dynamic-Array/src/dynamic-array.h"

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
void init_tc(uint32_t max_work, uint32_t n_clients, uint8_t use_return);

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
uint64_t tc_add_func(void *(*func)(void *), void *args);

/**
 * Returns the return value of the function with the given id
 * or NULL, if there is no such function.
 *
 * NOTE: you should call tc_join() becor calling this
 *       else the return value mybe not yet avilable
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
