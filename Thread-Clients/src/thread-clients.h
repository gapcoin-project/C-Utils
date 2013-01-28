#ifndef __THREAD_CLIENTS_H__
#define __THREAD_CLIENTS_H__

#include <pthread.h>

/* function id type */
typedef uint64_t tc_func_id_t

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
void init_tc(uint32_t max_work, uint32_t n_clients, uint8_t use_return);



#endif // __THREAD_CLIENTS_H__
