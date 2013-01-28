/**
 * smal definition of an variable length Array
 * with an max size and an custum content
 *
 * Its just like an Generic ArrayList from Java
 * exclusively the maximum size
 */
#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__
#include <time.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Generates an Long 64 Bit random number
 */
#define LONG_RAND                           \
  ((((((((((u_int64_t) rand()) << 15)       \
  + (u_int64_t) rand()) << 15)              \
  + (u_int64_t) rand()) << 15)              \
  + (u_int64_t) rand()) << 15)              \
  + (u_int64_t) rand() + 1) 

/**
 * Generates an random index for the given Array
 */
#define RAND_INDEX(ARY) LONG_RAND % (ARY).length

/**
 * defines an new Array struct with a given type as content
 * and a given Name
 */
#define DEFINE_ARRAY(TYPE, NAME)          \
typedef struct {                          \
  TYPE *ptr;                              \
  u_int64_t length;                       \
  u_int64_t max_len;                      \
} NAME

/**
 * returns the Element at the given possition
 */
#define ARY_AT(ARY, INDEX) (ARY).ptr[INDEX]

/**
 * returns the last Element of the given ARY
 */
#define ARY_LAST(ARY) ARY_AT(ARY, (ARY).length - 1)

/**
 * Initialize an given Array with an given start length
 * also initialize rand
 */
#define ARY_INIT(TYPE, ARY, START_LEN)                                \
  do {                                                                \
    (ARY).ptr     = (TYPE *) malloc(sizeof(TYPE) * (START_LEN));      \
    (ARY).length  = 0;                                                \
    (ARY).max_len = START_LEN;                                        \
    srand(time(NULL) * rand());
  } while (0)

/**
 * Insert an given element at the end of a given Array if possible
 *
 * NOTE: the Array content type should be the same as the type of 
 *       the given element!
 */
#define ARY_ADD(ARY, E)                                                   \
  do {                                                                    \
    if (ARY.length >= ARY.max_len) {                                      \
      ARY.ptr = realloc(ARY.ptr, ARY.length * 2 * sizeof(ARY.ptr[0]));    \
      ARY.max_len = ARY.length * 2;                                       \
    }                                                                     \
    ARY.ptr[ARY.length] = E;                                              \
    ARY.length++;
  } while (0)

/**
 * grows if neccesary 
 * (expecting you want to store an Element at Index)
 */
#define ARY_GROW(ARY, INDEX)                                                \
  do {                                                                      \
    while ((INDEX) >= (ARY).max_len) {                                      \
      (ARY).ptr = realloc( (ARY).ptr,                                       \
                           (ARY).max_len * 2 * sizeof((ARY).ptr[0]) );      \
      (ARY).max_len *= 2;                                                   \
    }                                                                     
  } while (0)

/**
 * Insert an given element at the end of a given Array
 *
 * Note it my reallocates space if array is to smal
 */
#define ARY_PUSH(ARY, E)                                                  \
  do {                                                                    \
    if (ARY.length >= ARY.max_len) {                                      \
      void *ptr = realloc( (ARY).ptr,                                     \
                           sizeof((ARY).ptr[0]) * (ARY).max_len * 2 );    \
      (ARY).max_len *= 2;                                                 \
    }                                                                     \
    (ARY).ptr[(ARY).length] = E;                                          \
    (ARY).length++;                                                       \
  } while (0)
  
/**
 * Removes and stores the last element from ARY in E
 */
#define ARY_PULL(ARY, E)                    \
  do {                                      \
    if ((ARY).length > 0) {                 \
      (ARY).length--;                       \
      E = (ARY).ptr[(ARY).length];          \
    }                                       \
  } while (0)

/**
 * Extract a random element from a given array
 * and saves it into a given value if possible
 *
 * note it changes the order of the given array
 * NOTE: the Arry content type should be the same as th type of 
 *       the given value
 */
#define ARY_EXTRACT(ARY, VALUE)                                   \
  do {                                                            \
    if ((ARY).length > 0) {                                       \
      uint64_t ary_ext_i = RAND_INDEX(ARY);                       \
      VALUE = (ARY).ptr[ary_ext_i];                               \
      (ARY).ptr[ary_ext_i] = (ARY).ptr[(ARY).length - 1];         \
      (ARY).length--;                                             \
    }                                                             \
  } while (0)


/**
 * Deletes al instaces of the given Element from th given Array
 * NOTE the Arry content type should be the same as th type of 
 * the given element
 */
#define ARY_DELETE(ARY, E)                                          \
  do {                                                              \
    u_int64_t adi, new_len;                                         \
    for (adi = new_len = 0; adi < (ARY).length; adi++) {            \
      (ARY).ptr[new_len] = (ARY).ptr[adi];                          \
      if (ARY).ptr[adi] != (E) { new_len++; }                       \
    }                                                               \
    (ARY).length = new_len;                                         \
  } while (0)

/**
 * frees an given ARY
 */
#define ARY_FREE(ARY) free((ARY).ptr)

/**
 * Returns wheter the given Array is empty or not
 */
#define EMPTY(ARY) ((ARY).length == 0)
#define NOTEMPTY(ARY) ((ARY).length != 0)

#endif // __DYNAMIC_ARRAY_H__
