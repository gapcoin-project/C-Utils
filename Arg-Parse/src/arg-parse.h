/**
 *  simple Argumnet parsing functions
 */
#ifndef __ARG_PARSE_H__
#define __ARG_PARSE_H__

/**
 * Returns if the given args conatinig the given arg
 */
char has_arg(int argc, 
             char *argv[], 
             const char *short_arg, 
             const char *long_arg);

/**
 * Returns the given argument arg
 */
char *get_arg(int argc, 
              char *argv[], 
              const char *short_arg, 
              const char *long_arg);

#endif /* __ARG_PARSE_H__ */
