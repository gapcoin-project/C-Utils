/**
 *  just a bunch of string manipulation functions
 */
#ifndef __STRING__
#define __STRING__

#include "string.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <regex.h>
#include <stdio.h>

/**
 * reads a line from a given file descriptor
 * returns line length or -1 by error
 */
int read_line(int fp, char *buffer) {

  int ret = 1, len = 0;
  
  
  /* loop until end of line or file */
  do {
    ret = read(fp, buffer + len, 1);

    /* error */
    if (ret == -1) 
      return -1;
    
    len++;
  } while (buffer[len - 1] != '\n' && ret != 0 );

  if (ret == 0)
    len--;

  buffer[len] = '\x00';
  return len;

}

/**
 * Splits a String by a specific char
 */
char **split(char *str, const char *seperator) {
  
  char *copy = str_clone(str);
  char *ptr = strtok(copy, seperator);

  int len = 2;
  char **splited = malloc(sizeof(char *) * len);

  int i;
  for (i = 0; ptr != NULL; i++) {
    
    int size = strlen(ptr);
    splited[i] = malloc(sizeof(char) * size + 1);
    strncpy(splited[i], ptr, size);
    splited[i][size] = '\0';

    ptr = strtok(NULL, seperator);
    
    if (i == len - 1) {
      len *= 2;
      splited = realloc(splited, sizeof(char *) * len);
    }
  }

  splited[i] = NULL;
  free(copy);

  return splited;
}

/**
 * returns true if str1 starts with str2
 */
#define start_with(str1, str2) (strncmp(str2, str1, strlen(str2)) == 0)

/**
 * returns if str1 ends with str2
 */
#define end_with(str1, str2)                                                \
  (strncmp(str2, str1 + (strlen(str1) - strlen(str2)), strlen(str2)) == 0)

static uint8_t get_byte(const uint8_t byte, int i) {
  switch (i) {
    case 0: return byte & 1;
    case 1: return (byte & 2) >> 1;
    case 2: return (byte & 4) >> 2;
    case 3: return (byte & 8) >> 3;
    case 4: return (byte & 16) >> 4;
    case 5: return (byte & 32) >> 5;
    case 6: return (byte & 64) >> 6;
    case 7: return (byte & 128) >> 7;
  }

  return -1;
}


/**
 * CRC32 implementation
 */
uint32_t str_crc32(const char *str, uint32_t len) {
  
  uint32_t shift = 0x0;
  uint32_t crc  = 0x04C11DB7;
  
  uint32_t i;
  for (i = 0; i < len * 8; i++) {
    if (((shift & (1 << 31)) >> 31) != get_byte((uint8_t) str[i / 8], i % 8))
      shift = (shift << 1) ^ crc;
    else
      shift <<= 1;
  }
  
  return shift;
}

/**
 * clone a string
 */
char *str_clone(const char *str) {

  char *clone;
  size_t len = strlen(str);

  clone = malloc(len + 1);
  memcpy(clone, str, len + 1);

  return clone;
}

/**
 * clone a string
 */
char *strn_clone(const char *str, size_t n) {

  char *clone;

  clone = malloc(n);
  memcpy(clone, str, n);

  return clone;
}

/**
 * returns the possition of the first new line character in the given string
 */
static inline ssize_t new_line_pos(char *str, size_t len) {
  
  size_t i;
  for (i = 0; i < len && str[i] != '\n'; i++);

  return i;
}

/**
 * reallocs new space if needed
 */
static inline ssize_t check_memory(char **buffer, ssize_t *capacity, ssize_t wanted) {

  if (*capacity < 1024) {
    *capacity = 1024;
    char *ptr = realloc(*buffer, *capacity);

    if (ptr == NULL)
      return -1;

    *buffer = ptr;
  }
  
  while (*capacity < wanted) {
    *capacity *= 2;
    
    char *ptr = realloc(*buffer, *capacity);

    if (ptr == NULL)
      return -1;

   *buffer = ptr;
  }

  return 0;
}

/**
 * recives on line form a given socket file descriptor
 * Note: it reallocs space if needed
 * NOTE: *buffer hav to be an dynamic allocated address
 */
ssize_t recv_line(int sock_fd, char **buffer, ssize_t *capacity, int flags) {
  
  static char recv_buff[1024];
  static ssize_t recved_size = 0;

  ssize_t size = 0, size_one = 0;

  for (;;) {
    
    /* malloc new space if needed */
    if (check_memory(buffer, capacity, size + 1023) == -1)
      return -1;

    /* recev on portion form sender or use old recvied portion */
    if (recved_size > 0) {
      
      memmove(*buffer, recv_buff, recved_size);
      size_one = recved_size;
      recved_size = 0;
    } else
      size_one = recv(sock_fd, (*buffer) + size, 1023, flags);

    /* check for errors */
    if (size_one == -1) return -1;

    /* search fo an new line char */
    ssize_t new_line_char = new_line_pos((*buffer) + size, size_one);

    /* adjust size */
    new_line_char += size;
    size += size_one;

    /* we found a new line char */
    if (size > new_line_char) {
      
      /* part of the next line */
      recved_size = size - (new_line_char + 1);

      /* save part of the next line for futer calls */
      if (recved_size > 0)
        memmove(recv_buff, (*buffer) + new_line_char + 1, recved_size);

      (*buffer)[new_line_char] = 0x0;

      size = new_line_char;
      break;
    }
  } 

  return size;
}

/**
 * returns wheretre the given string matches the given regex
 */
char str_matches(const char *regex_str, const char *str) {
  
  regex_t regex;

  int ret = regcomp(&regex, regex_str, REG_NOSUB);

  if (ret)
    return ret;

  if (regexec(&regex, str, 0, NULL, 0))
    return -1;
  else
    return 1;
}

/**
 * converts an byte array to base64
 * Note tere should be more memory avilabel in the given array
 * cause there will my be added filling bytes
 */
char *to_b64(uint8_t *bytes, uint64_t len, char *b64) {
  
  static const char code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789+/";

  /* number of filling bytes */
  uint64_t n_fill = (3 - (len % 3)) % 3;
  uint64_t b64_len = ((len + n_fill) / 3) * 4 + n_fill;

  
  if (b64 == NULL)
    b64 = malloc(sizeof(char) * (b64_len + 1));

  /* adding fill bytes */
  uint64_t i;
  for (i = 0; i < n_fill; i++)
    bytes[len + i] = 0;

  len += n_fill;

  for (i = 0; i < len; i += 3) {
    b64[(i / 3) * 4]     = code[bytes[i] >> 2];
    b64[(i / 3) * 4 + 1] = code[((bytes[i] & 3) << 4) | (bytes[i + 1] >> 4)];
    b64[(i / 3) * 4 + 2] = code[((bytes[i + 1] & 0xF) << 2) |
                                (bytes[i + 2] >> 6)];
    b64[(i / 3) * 4 + 3] = code[bytes[i + 2] & 0x3F];
  }

  for (i = (i / 3) * 4; i < b64_len; i++)
    b64[i] = '=';

  b64[b64_len] = '\0';
  return b64;  
}

/**
 * returns the decode length of an base64 encoded string
 * (no new line chars are allowed)
 */
size_t b64_dec_len(const char *b64) {
  
  size_t n_fill = 0;
  size_t len    = strlen(b64);

  if (b64[len - 1] == '=') {
    n_fill++;

    if (b64[len - 2] == '=')
      n_fill++;
  }

  return (len / 4) * 3 - n_fill;
}

/**
 * decodes an b64 encoded string
 * (no new line chars are allowed)
 */
uint8_t *b64_to_byte(const char *b64, 
                            uint8_t *bytes) {
  
  /**
   * Array which orders the base 64 elements to ist code
   */
  static const uint8_t code[] = { 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 
    61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 
    45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, };

  size_t n_fill  = 0;
  size_t b64_len = strlen(b64);

  if (b64[b64_len - 1] == '=') {
    n_fill++;

    if (b64[b64_len - 2] == '=')
      n_fill++;
  }

  size_t byte_len = (b64_len / 4) * 3 - n_fill;

  if (bytes == NULL)
    bytes = malloc(sizeof(uint8_t) * byte_len + 1);

  
  size_t i;
  for (i = 0; i < b64_len; i += 4) {
    bytes[(i / 4) * 3]      = (code[(uint8_t) b64[i]] << 2)     |
                              (code[(uint8_t) b64[i + 1]] >> 4);
    bytes[(i / 4) * 3 + 1]  = (code[(uint8_t) b64[i + 1]] << 4) |
                              (code[(uint8_t) b64[i + 2]] >> 2);
    bytes[(i / 4) * 3 + 2]  = (code[(uint8_t) b64[i + 2]] << 6) |
                               code[(uint8_t) b64[i + 3]];
  }

  return bytes;
}

/**
 * return s if the given string is base64 encoded
 * (no new line chars are allowed)
 */ 
char is_b64(const unsigned char *str) {

  /**
   * Array which orders the base 64 elements to ist code
   */
  static const uint8_t code[] = { 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 
    61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 
    45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, };

  size_t i;
  size_t len = strlen((const char *) str);
  for (i = 0; i < len; i++) 
    if (code[str[i]] == (uint8_t) -1)
      break;

  if (i != len) {
    if (i == len - 1 && str[i] == '=')
      return 1;
    else if (i == len - 2 && str[i] == '=' && str[i + 1] == '=')
      return 1;
    else 
      return 0;
  }
  
  return 1;
}

/**
 * Puts a given string in revers order
 */
void str_reverse(char *str) {
  
  char tmp = 0;

  size_t i, j;
  for (i = 0, j = strlen(str) - 1; i < j; i++, j--) {
    tmp = str[i];
    str[i] = str[j];
    str[j] = tmp;
  }
}

/**
 * converts a given integer to a string
 */
char *itoa(int64_t i) {

  char *a = malloc(sizeof(char) * 22);

  if (i == 0) {
    a[0] = '0';
    a[1] = '\0';
    return a;
  }

  char is_neg = (i < 0);

  if (is_neg)
    i *= -1;

  int j;
  for (j = 0; i != 0; j++, i /= 10)
    a[j] = 48 + (i % 10);

  if (is_neg) {
     a[j] = '-';
    j++;
  }

  a[j] = '\0';
  str_reverse(a);
  return a;
}

/**
 * reads and returns entire file 
 */
BinaryString *read_file(int fd) {

  BinaryString *bstr = malloc(sizeof(BinaryString));

  if (bstr == NULL) return NULL;

  /* get file length */
  struct stat file_info;

  if(fstat(fd, &file_info) == -1)
    return NULL;

  bstr->len = file_info.st_size;
  bstr->str = malloc(sizeof(char) * file_info.st_size);

  if (bstr->str == NULL) 
    return NULL;

  if (read(fd, bstr->str, bstr->len) == -1)
    return NULL;
  
  return bstr;
}

/**
 * creates an readable string from a uint64_t
 */
char *ui_to_str(uint64_t num) {
  
  char buffer[64];

  if (num == 0) {
    buffer[0] = '0';
    buffer[1] = 0;
    return str_clone(buffer);
  }

  uint64_t i, j;
  for (i = 0, j = 0; num != 0 && i < 64; num /= 10, i++, j++) {
    
    buffer[i] = 48 + num % 10;

    if (i != 0 && (j + 1) % 3 == 0) {
      i++;
      buffer[i] = 39;
    }
  }

  if (buffer[i - 1] == 39)
    i--;

  buffer[i] = 0;
  str_reverse(buffer);

  return str_clone(buffer);
}

/**
 * hexdumps the given string to the given file descriptor
 */
void hexdump(unsigned char *str, int len) {
  
  int i, n;
  for (i = 0; i < len; i++) {
    if (i % 8 == 0 && i > 0)
      printf(" ");
    if (i % 16 == 0 && i > 0) {
      printf(" |");
      
      for (n = i - 16; n < i; n++) {
        if (str[n] >= 32 && str[n] <= 126)
          printf("%c", str[n]);
        else
          printf(".");
      }
      printf("|\n");
    }
    printf("%02x ", str[i]);
  }

  for (; ((i % 16) - 1) != 0; i++) {
    if (i % 8 == 0 && i > 0)
      printf(" ");
    if (i % 16 == 0 && i > 0) {
      printf(" |");
      
      for (n = i - 16; n < len; n++) {
        if (str[n] >= 32 && str[n] <= 126)
          printf("%c", str[n]);
        else
          printf(".");
      }

      for (; n < i; n++) {
        printf(" ");
      }
      printf("|\n");
    }

    if (i % 16 != 0)
      printf("   ");
  }
  printf("\n");
}

#endif /* __STRING__ */
