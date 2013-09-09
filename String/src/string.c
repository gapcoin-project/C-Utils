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

    ptr = strtok(NULL, seperator);
    
    if (i == len - 1) {
      len *= 2;
      splited = realloc(splited, sizeof(char *) * len);
    }
  }

  splited[i] = malloc(sizeof(char) * 2);
  splited[i][0] = 0;

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

#endif /* __STRING__ */
