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
char str_matches(const char *regex_str, const char *str);

/**
 * converts an byte array to base64
 * Note tere should be more memory avilabel in the given array
 * cause there will my be added filling bytes
 */
inline char *to_b64(uint8_t *bytes, const uint64_t len);

/**
 * returns the length of an base64 encoded string
 * (no new line chars are allowed)
 */
inline size_t b64_len(const char *b64);

/**
 * decodes an b64 encoded string
 * (no new line chars are allowed)
 */
#define b64_to_byte1(b64) b64_to_byte(b64, b64_len(b64))
#define b64_to_byte2(b64, len) b64_to_byte(b64, len)
#define b64_to_bytex(x, b1, b2, func, ...) func
#define b64_to_byte(...) b64_to_bytex(, ##__VA_ARGS__,                \
                                        b64_to_byte2(__VA_ARGS__),    \
                                        b64_to_byte1(__VA_ARGS__))
inline uint8_t *b64_to_byte(const char *b64, const uint64_t len);

#endif /* __STRING_H__ */
