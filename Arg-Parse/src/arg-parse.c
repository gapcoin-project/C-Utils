/**
 *  simple Argumnet parsing functions
 */
#ifndef __ARG_PARSE__
#define __ARG_PARSE__

#include <string.h>

/**
 * Returns if the given args conatinig the given arg
 */
char has_arg(int argc, 
             char *argv[], 
             const char *short_arg, 
             const char *long_arg) {

  int i;
  for (i = 1; i < argc; i++) {
    if (!strcmp(argv[i], short_arg) ||
        !strcmp(argv[i], long_arg)) {
      
      return 1;
    }
  }

  return 0;
}

/**
 * Returns the given argument arg
 */
char *get_arg(int argc, 
              char *argv[], 
              const char *short_arg, 
              const char *long_arg) {

  int i;
  for (i = 1; i < argc - 1; i++) {
    if (!strcmp(argv[i], short_arg) ||
        !strcmp(argv[i], long_arg)) {
      
      return argv[i + 1];
    }
  }

  return NULL;
}

#endif /* __ARG_PARSE__ */
