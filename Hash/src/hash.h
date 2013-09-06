/**
 *  a simple hash map implementation, macro based
 */
#ifndef __HASH_H__
#define __HASH_H__

#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

/**
 * Simple test if a number is a pime number
 * (i know it is very slow but works god til ~ 2^30)
 */
static inline char is_prime(uint32_t n) {

  if (n < 2) return 0;
  if (n < 4) return 1;
  if (n % 2 == 0) return 0;

  uint32_t i;
  uint32_t limit = sqrt(n) + 1;
  for (i = 3; i < limit; i+= 2)
    if (n % i == 0) return 0;

  return 1;
}

/**
 * Calculates and returns the next bigger pimnumber
 */
static inline uint32_t next_prime(uint32_t n) {
  
  while (is_prime(n) == 0)
    n++;

  return n;
}

/**
 * defines a new hash with the given element type and name
 */
#define DEF_HASH(TYPE, NAME)                          \
typedef struct NAME NAME;                             \
struct NAME {                                         \
  TYPE     *ptr;                                      \
  uint32_t *kys;                                      \
  uint32_t len;                                       \
  uint32_t n_elements;                                \
  /* the has constant for                             \
   * the hastfunctions */                             \
  const double h1;                                    \
  const double h2;                                    \
  TYPE tmp_element; /* for easy sizeof */             \
  NAME *tmp_hash;   /* for cloning */                 \
} 

/**
 * initialize a given hash with a given minimum number of elements
 * (it realy sets the length to the next 2^n potenz because of speed)
 */
#define HASH_INIT(HASH, MIN_LEN)                                            \
do {                                                                        \
   (HASH).len = next_prime(MIN_LEN);                                        \
                                                                            \
  (HASH).ptr = malloc(sizeof((HASH).tmp_element) * (HASH).len);             \
  (HASH).kys = malloc(sizeof(uint32_t) * (HASH).len);                       \
                                                                            \
  /* -1 and -2 are not allowed values */                                    \
  memset((HASH).kys, (uint32_t) -1, sizeof(uint32_t) * (HASH).len);         \
  memset(&(HASH).tmp_element, (uint32_t) -1, sizeof((HASH).tmp_element));   \
  (HASH).n_elements = 0;                                                    \
                                                                            \
  /** initializing h1, and h1 like Knut says */                             \
  *(double *) &(HASH).h1 = (sqrt(5.0) - 1.0) / 2.0;                         \
  *(double *) &(HASH).h2 = (sqrt(7.0) - 1.0) / 2.0;                         \
} while (0)

/**
 * clones src into destination hash
 */
#define HASH_CLONE(DST, SRC)                                                \
do {                                                                        \
  if ((DST).len == (SRC).len) {                                             \
    memcpy((DST).ptr, (SRC).ptr, (SRC).len * sizeof((SRC).ptr[0]));         \
    memcpy((DST).kys, (SRC).kys, (SRC).len * sizeof((SRC).ptr[0]));         \
                                                                            \
    (DST).n_elements = (SRC).n_elements;                                    \
                                                                            \
    *((double *) &(DST).h1) = (SRC).h1;                                     \
    *((double *) &(DST).h2) = (SRC).h2;                                     \
  }                                                                         \
} while (0)

/**
 * frees all resources of a given hash
 */
#define HASH_FREE(HASH)   \
do {                      \
  free((HASH).ptr);       \
  free((HASH).kys);       \
} while (0)

/**
 * Returns the decimal part of an double
 */
#define DOUBLE_DEC(DOUBLE)              \
  ((DOUBLE) - ((int32_t) (DOUBLE)))

/**
 * The has function, it calculates the actual slot out of a given key,
 * and index (number of already taken slots)
 * NOTE: key has to be an integer
 */
#define HASH_I(HASH, KEY, I)                                                  \
 ((((uint32_t) ((HASH).len * DOUBLE_DEC(((KEY) * (HASH).h1)))) +              \
   ((uint32_t) (1 + (((HASH).len -1) *                                        \
                     DOUBLE_DEC(((KEY) * (HASH).h2)))) * (I))) %              \
  (HASH).len)

/**
 * stores the element with the given key form the given hash
 * in E
 */
#define HASH_GET(HASH, KEY, E)                                               \
do {                                                                         \
  uint32_t __hash_i__;                                                       \
                                                                             \
  for (__hash_i__ = 0;                                                       \
      __hash_i__ < (HASH).len &&                                             \
      ((HASH).kys[HASH_I(HASH, KEY, __hash_i__)] != (KEY)) &&                \
      ((HASH).kys[HASH_I(HASH, KEY, __hash_i__)] != (uint32_t) -1);          \
      __hash_i__++);                                                         \
                                                                             \
  if ((HASH).kys[HASH_I(HASH, KEY, __hash_i__)] != (uint32_t) -1)            \
    E = (HASH).ptr[HASH_I(HASH, KEY, __hash_i__)];                           \
                                                                             \
} while (0)

/** 
 * inserts an given element with a given key into a given hash
 * (internal version with out check of hash is full)
 */
#define HASH_ADD_INTERN(HASH, KEY, E)                                        \
do {                                                                         \
                                                                             \
  uint32_t __hash_i__;                                                       \
                                                                             \
  for (__hash_i__ = 0;                                                       \
      __hash_i__ < (HASH).len &&                                             \
      ((HASH).kys[HASH_I(HASH, KEY, __hash_i__)] != (uint32_t) -1);          \
      __hash_i__++);                                                         \
                                                                             \
  (HASH).ptr[HASH_I(HASH, KEY, __hash_i__)] = E;                             \
  (HASH).kys[HASH_I(HASH, KEY, __hash_i__)] = KEY;                           \
  (HASH).n_elements++;                                                       \
                                                                             \
} while (0)                                                                 
                                                                            

/**
 * grows an HASH inf neccesary
 */
#define HASH_GROW(HASH)                                                       \
do {                                                                          \
  if (((double) (HASH).n_elements / (double) (HASH).len) >= 0.9) {  \
                                                                              \
    (HASH).tmp_hash = malloc(sizeof(HASH));                                   \
    HASH_INIT(*(HASH).tmp_hash, (HASH).len);                                  \
    HASH_CLONE(*(HASH).tmp_hash, HASH);                                       \
                                                                              \
    HASH_FREE(HASH);                                                          \
    HASH_INIT(HASH, (HASH).len * 2);                                          \
                                                                              \
    uint32_t __hash_i__, __hash_ky__;                                         \
                                                                              \
    for (__hash_i__ = 0; __hash_i__ < (HASH).tmp_hash->len; __hash_i__++) {   \
                                                                              \
      __hash_ky__ = (HASH).tmp_hash->kys[__hash_i__];                         \
                                                                              \
      if (__hash_ky__ != (uint32_t) -1) {                                     \
                                                                              \
        HASH_GET(*(HASH).tmp_hash, __hash_ky__, (HASH).tmp_element);          \
        HASH_ADD_INTERN(HASH, __hash_ky__, (HASH).tmp_element);               \
      }                                                                       \
    }                                                                         \
                                                                              \
    /* clean up */                                                            \
    HASH_FREE(*(HASH).tmp_hash);                                               \
    free((HASH).tmp_hash);                                                    \
  }                                                                           \
} while (0)

/**
 * inserts a given element with a given key into the given hash
 * Note: it grows the hash if neccesary
 */
#define HASH_ADD(HASH, KEY, E)          \
do {                                    \
  HASH_GROW(HASH);                      \
  HASH_ADD_INTERN(HASH, KEY, E);        \
} while (0)

/**
 * adds an elemnt to the given hash unsing a string as key
 */
#define HASH_ADD_STR(HASH, STR, E)                          \
  HASH_ADD(HASH, crc32(0, (uint8_t *) STR, strlen(STR)), E)

/**
 * saves the wanted element in E uses a sting as key
 */
#define HASH_GET_STR(HASH, STR, E)                          \
  HASH_GET(HASH, crc32(0, (uint8_t *) STR, strlen(STR)), E)


#endif /* __HASH_H__ */
