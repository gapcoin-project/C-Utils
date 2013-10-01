/**
 *  some memory related functions
 */
#ifndef __MEMORY__
#define __MEMORY__

#include "memory.h"
#include <unistd.h>
#include <stdio.h>

/**
 * Returns the Total Memory of the underlying System
 */
uint64_t get_total_memory_size() {

  uint64_t pages = sysconf(_SC_PHYS_PAGES);
  uint64_t page_size = sysconf(_SC_PAGE_SIZE);
  return pages * page_size;
}

/**
 * returns the human readable Bytes as a String 
 */
char* readable_fs(long double size, char *buf) {

  int i = 0;
  const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};

  while (size >= 1000.0) {
      size /= 1024.0;
      i++;
  }

  sprintf(buf, "%5Lf %s", size, units[i]);
  return buf;
}


#endif /* __MEMORY__ */
