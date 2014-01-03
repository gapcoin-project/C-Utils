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

  switch (i % 8) {                                        
    case 0 : bary->ptr[i / 8] |= BIT0; break;       
    case 1 : bary->ptr[i / 8] |= BIT1; break;       
    case 2 : bary->ptr[i / 8] |= BIT2; break;       
    case 3 : bary->ptr[i / 8] |= BIT3; break;       
    case 4 : bary->ptr[i / 8] |= BIT4; break;       
    case 5 : bary->ptr[i / 8] |= BIT5; break;       
    case 6 : bary->ptr[i / 8] |= BIT6; break;       
    case 7 : bary->ptr[i / 8] |= BIT7; break;       
  }                                                         
}
    
/**
 * Unsets the given Possition
 */
static inline void bary_unset(BoolArray *bary, uint64_t i) {
                                                                  
  switch (i % 8) {                                            
    case 0 : bary->ptr[i / 8] &= ~BIT0; break;          
    case 1 : bary->ptr[i / 8] &= ~BIT1; break;          
    case 2 : bary->ptr[i / 8] &= ~BIT2; break;          
    case 3 : bary->ptr[i / 8] &= ~BIT3; break;          
    case 4 : bary->ptr[i / 8] &= ~BIT4; break;          
    case 5 : bary->ptr[i / 8] &= ~BIT5; break;          
    case 6 : bary->ptr[i / 8] &= ~BIT6; break;          
    case 7 : bary->ptr[i / 8] &= ~BIT7; break;          
  }                                                             
}

/**
 * returns wether the given index is setted or not
 */
static inline uint8_t bary_at(BoolArray *bary, uint64_t i) {

  switch (i % 8) {                                        
    case 0 : return bary->ptr[i / 8] & BIT0;       
    case 1 : return bary->ptr[i / 8] & BIT1;       
    case 2 : return bary->ptr[i / 8] & BIT2;       
    case 3 : return bary->ptr[i / 8] & BIT3;       
    case 4 : return bary->ptr[i / 8] & BIT4;       
    case 5 : return bary->ptr[i / 8] & BIT5;       
    case 6 : return bary->ptr[i / 8] & BIT6;       
    case 7 : return bary->ptr[i / 8] & BIT7;       
  }                                                         

  printf("SOMETHING WENT WRONG IN: %s:%d\n", __FILE__, __LINE__);
  exit(EXIT_FAILURE);
}


#endif // __BARY_H__
