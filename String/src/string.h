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

/**
 * clone a string
 */
inline char *str_clone(const char *str);

/**
 * clone a string
 */
inline char *strn_clone(const char *str, size_t n);

/**
 * receve one line from a socket fd
 */
inline ssize_t recv_line(int sock_fd, char *buffer, size_t len, int flags);

/**
 * returns wheretre the given string matches the given regex
 */
char matches(const char *regex_str, const char *str);

#endif /* __STRING_H__ */
