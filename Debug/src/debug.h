/**
 * some helpfull debunging Stuff to make code smaler
 * and work easier
 */
#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <stdio.h>
#include <stdlib.h>

/**
 * prints a debuging Message with file and line
 * if DEBUG flag are setted
 */
#ifdef DEBUG
#define DBG_MSG(MSG)                                                    \
  printf("[DEBUG] %s, in %s: %d\n", (MSG), __FILE__, __LINE__)
#else
#define DBG_MSG(MSG) (void) (MSG)
#endif

/**
 * prints a Visible WARNING Message with file and line
 * if DEBUG flag are setted
 */
#ifdef DEBUG
#define WARN_MSG(MSG)                                                     \
  do {                                                                    \
    printf("[WARNING] %s, in %s: %d!!!\n", (MSG), __FILE__, __LINE__);    \
    printf("[WARNING] %s, in %s: %d!!!\n", (MSG), __FILE__, __LINE__);    \
    printf("[WARNING] %s, in %s: %d!!!\n", (MSG), __FILE__, __LINE__);    \
  } while (0)
#else
#define WARN_MSG(MSG) (void) (MSG)
#endif

/**
 * prints a Error message and exists Message with file and line
 * if DEBUG flag are setted
 */
#ifdef DEBUG
#define ERR_MSG(MSG)                                                      \
  do {                                                                    \
    printf("[ERROR] %s, in %s: %d!!!\n", (MSG), __FILE__, __LINE__);      \
    exit(1);                                                              \
  } while (0)
#else
#define ERR_MSG(MSG) (void) (MSG)
#endif

/**
 * verbose Messages
 */
#ifdef VERBOSE
#define INFO_MSG(MSG) printf("[II] " MSG)
#else
#define INFO_MSG(MSG) (void) (MSG)
#endif

#endif // __DEBUG_H__
