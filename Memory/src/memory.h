/**
 *  some memory related functions
 */
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <inttypes.h>

/**
 * Returns the Total Memory of the underlying System
 */
uint64_t get_total_memory_size();

/**
 * returns the human readable Bytes as a String 
 */
char* readable_fs(long double size, char *buf);

#endif /* __MEMORY_H__ */
