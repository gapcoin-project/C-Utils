/**
 * Header for an Bool Array
 * Minimizes spaces if only True or False is Neccesbary
 */
#ifndef __BARY_H__
#define __BARY_H__

#include <sys/types.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  uint8_t *ptr;
  uint64_t len;
  uint8_t init;
} BoolArray;

#define BIT0 1
#define BIT1 2
#define BIT2 4
#define BIT3 8
#define BIT4 16
#define BIT5 32
#define BIT6 64
#define BIT7 128

/**
 * Inits Array with the given bool init value and the given length
 */
static inline void bary_init(BoolArray *bary, uint64_t len, uint8_t init) {
  
  bary->ptr  = malloc(sizeof(uint8_t) * len / 8);
  bary->len  = len;
  bary->init = (init != 0) ? 255 : 0;

  memset(bary->ptr, bary->init, bary->len / 8 * sizeof(uint8_t));
}

/**
 * grows the size to a given index
 */
static inline void bary_grow(BoolArray *bary, uint64_t len) {
  
  while (bary->len <= len) {

    bary->len *= 2;
    bary->ptr      = realloc(bary->ptr, sizeof(uint8_t) * bary->len / 8);
    memset(bary->ptr + (bary->len / 2), 
           bary->init, 
           bary->len / 16 * sizeof(uint8_t));
  }
}

/**
 * Sets the given Possition
 */
static inline void bary_set(BoolArray *bary, uint64_t i) {

  bary->ptr[i / 8] |= (1 << (1 % 8));
}
    
/**
 * Unsets the given Possition
 */
static inline void bary_unset(BoolArray *bary, uint64_t i) {
                                                                  
  bary->ptr[i / 8] &= ~(1 << (1 % 8));
}

/**
 * returns wether the given index is setted or not
 */
static inline uint8_t bary_at(BoolArray *bary, uint64_t i) {

  return bary->ptr[i / 8] & (1 << (1 % 8));
}


#endif // __BARY_H__
