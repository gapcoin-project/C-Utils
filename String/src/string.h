/**
 *  just a bunch of string manipulation functions
 */
#ifndef __STRING_H__
#define __STRING_H__

#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/**
 * reads a line from a given file descriptor
 * returns line length or -1 by error
 */
int read_line(int fp, char *buffer);

#endif /* __STRING_H__ */
