/**
 *  just a bunch of string manipulation functions
 */
#ifndef __STRING_H__
#define __STRING_H__

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>

/**
 * reads a line from a given file descriptor
 * returns line length or -1 by error
 */
int read_line(int fp, char *buffer);

/**
 * Splits a String by a specific char
 */
char **split(char *str, const char *seperator);

/**
 * CRC32 implementation
 */
inline uint32_t crc32(const char *str, uint32_t len);

#endif /* __STRING_H__ */
