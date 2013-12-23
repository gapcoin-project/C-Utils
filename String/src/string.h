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
 * struct for stroring information about an binary String
 */
typedef struct {
  char *str;
  uint64_t len;
} BinaryString;

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
uint32_t str_crc32(const char *str, uint32_t len);

/**
 * clone a string
 */
char *str_clone(const char *str);

/**
 * clone a string
 */
char *strn_clone(const char *str, size_t n);

/**
 * recives on line form a given socket file descriptor
 * Note: it reallocs space if needed
 * NOTE: *buffer hav to be an dynamic allocated address
 */
ssize_t recv_line(int sock_fd, char **buffer, ssize_t *capacity, int flags);

/**
 * returns wheretre the given string matches the given regex
 */
char str_matches(const char *regex_str, const char *str);

/**
 * converts an byte array to base64
 * Note tere should be more memory avilabel in the given array
 * cause there will my be added filling bytes
 */
#define to_b641(bytes, len) to_b64(bytes, len, NULL)
#define to_b642(bytes, len, b64) to_b64(bytes, len, b64)
#define to_b64x(x, y, b1, b2, func, ...) func
#define to_b64(...) to_b64x(, ##__VA_ARGS__,           \
                              to_b642(__VA_ARGS__),    \
                              to_b641(__VA_ARGS__))
char *to_b64(uint8_t *bytes, uint64_t len, char *b64);

/**
 * returns the decode length of an base64 encoded string
 * (no new line chars are allowed)
 */
size_t b64_dec_len(const char *b64);

/**
 * calculates the size an base64 encoded string will have
 */
#define b64_enc_len(len)                                                \
  (((len + ((3 - (len % 3)) % 3)) / 3) * 4 + ((3 - (len % 3)) % 3))

/**
 * decodes an b64 encoded string
 * (no new line chars are allowed)
 */
#define b64_to_byte1(b64) b64_to_byte(b64, NULL)
#define b64_to_byte2(b64, bytes) b64_to_byte(b64, bytes)
#define b64_to_bytex(x, b1, b2, func, ...) func
#define b64_to_byte(...) b64_to_bytex(, ##__VA_ARGS__,                \
                                        b64_to_byte2(__VA_ARGS__),    \
                                        b64_to_byte1(__VA_ARGS__))
uint8_t *b64_to_byte(const char *b64, 
                            uint8_t *bytes);

/**
 * return s if the given string is base64 encoded
 * (no new line chars are allowed)
 */ 
char is_b64(const unsigned char *str);

/**
 * removes the last character of an string
 */
#define str_chop(str) str[strlen(str) - 1] = '\0'

/**
 * converts a given integer to a string
 */
char *itoa(int64_t i);

/**
 * Puts a given string in revers order
 */
void str_reverse(char *str);

/**
 * returns if the given string ends with the given second given string
 */
#define str_end_with(str1, str2)                      \
  !strcmp(str1 + strlen(str1) - strlen(str2), str2)

/**
 * reads and returns entire file 
 */
BinaryString *read_file(int fd);

#endif /* __STRING_H__ */
