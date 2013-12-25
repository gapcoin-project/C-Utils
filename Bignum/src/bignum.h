/**
 * my own unsigned Bignum library
 */
#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/**
 * exit on error
 */
#define perror(msg)                 \
do {                                \
  perror(msg);                      \
  exit(EXIT_FAILURE);               \
} while (0)

/**
 * the underliying data type to store the bignums
 * NOTE: only unsigend types allowed
 */
#ifndef big_t
#define big_t uint32_t
#endif

/**
 * max numer fit into big_t (remeber only unsigend types allowed)
 */
#define big_max ((big_t) -1)

/**
 * the bignum type
 */
typedef struct {
  big_t *ptr;
  uint_fast32_t big_len; /* number of highest ptr index in use */
  uint_fast32_t len; /* byte length of ptr */
} Bignum;

/**
 * checks and addjusts the size of an bignum if needed
 */
static inline void big_grow(Bignum *b, uint_fast32_t len) {
  
  if (b->len < len) {
    big_t *ptr = realloc(b->ptr, b->len * 2);

    if (ptr == NULL)
      perror("failed to alloc space in big_grow");

    b->ptr = ptr;

    /* clear the new alloced space */
    memset(ptr + b->len, 0, b->len);

    /* save new len */
    b->len *= 2;
  }
}

/**
 * addition of two bignums
 */
static inline void big_add(Bignum *res, Bignum *a, Bignum *b) {

  /* adjust sizes,  + 1 because of possible carry */
  uint_fast32_t longes = (a->big_len < b->big_len) ? b->big_len : a->big_len;
  longes++;

  big_grow(res, longes);
  big_grow(a, longes);
  big_grow(b, longes);

  uint_fast32_t i, carry_old, carry;
  for (i = 0, carry = 0, carry_old = 0; i < longes; i++) {
    
    /* result without carry */
    res->ptr[i] = a->ptr[i] + b->ptr[i];

    /* check for new carry */
    carry = (res->ptr[i] < a->ptr[i] || res->ptr[i] < b->ptr[i]);

    /* check if old carry causes new carry */
    if (carry_old && res->ptr[i] == big_max)
      carry = 1;

    /* add old carry */
    res->ptr[i] += carry_old;

    carry_old = carry;
  }
}


#endif /* __BIGNUM_H__ */
