#ifndef __THREAD_CLIENTS_H__
#define __THREAD_CLIENTS_H__

#include <pthread.h>


/**
 * Initializes Thread-Clients Lib
 *
 * Parameters:
 *    max_work:   the expected maximum among of
 *                function given at a time to process
 *    n_clients:  the number of clients (threads) to use
 */
void init_tc(uint32_t max_work, uint32_t n_clients);



#endif // __THREAD_CLIENTS_H__
