/**
 *  just a bunch of string manipulation functions
 */
#ifndef __STRING__
#define __STRING__

#include "string.h"
#include <stdlib.h>

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
  
  char *ptr = strtok(str, seperator);

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

static inline uint8_t get_byte(const uint8_t byte, int i) {
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
inline uint32_t crc32(const char *str, uint32_t len) {
  
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

#endif /* __STRING__ */
