/**
 * Justa n implementation of som Quicksort variants
 */
#ifndef CHIEFSORT
#define CHIEFSORT
#include "sort.h"


/**
 * Just Insertionsort, min element first
 * takes a void pointer to an array
 * length is the number of elements in that array
 * base is the size of an array element
 * smaler is function pointer to an smaler function
 * that compares two array elements the start address of
 * the elements are given as void pointers
 */
void insertionsort_min(void *ary, 
                       int64_t length, 
                       int32_t base, 
                       char (*smaler)(void *, void *)) {
  
  int64_t i, j;
  void  *temp = malloc(base);

  for (i = 1; i < length; i++) {
    memcpy(temp, ary + i * base, base);
    j = i - 1;

    while (j >= 0 && smaler(temp, ary + (j * base))) {
      memcpy(ary + (j + 1) * base, ary + j * base, base);
      j = j - 1;
    }   
    
    memcpy(ary + (j + 1) * base, temp, base);
  }
}

/**
 * Just Insertionsort, max element first
 * takes a void pointer to an array
 * length is the number of elements in that array
 * base is the size of an array element
 * bigger is function pointer to an bigger function
 * that compares two array elements the start address of
 * the elements are given as void pointers
 */
void insertionsort_max(void *ary, 
                       int64_t length, 
                       int32_t base, 
                       char (*bigger)(void *, void *)) {
  
  int64_t i, j;
  void  *temp = malloc(base);

  for (i = 1; i < length; i++) {
    memcpy(temp, ary + i * base, base);
    j = i - 1;

    while (j >= 0 && bigger(temp, ary + (j * base))) {
      memcpy(ary + (j + 1) * base, ary + j * base, base);
      j = j - 1;
    }   
    
    memcpy(ary + (j + 1) * base, temp, base);
  }
}

/**
 * Improofed variant of QuickSort which uses insertionsort 
 * at an specific min array size, sorting min element first
 *
 * ary is a void pointer to an array
 * length is the number of elements in that array
 * base is the size of an array element
 * smaler, bigger, equal is function pointer
 * that compares two array elements the start address of
 * the elements are given as void pointers
 * 
 */
void quickinsersort_min(void *ary, 
                        int64_t length, 
                        int16_t min, 
                        int32_t base, 
                        char (*smaler)(void *, void *),
                        char (*bigger)(void *, void *),
                        char (*equal)(void *, void *)) {

  void *piv  = malloc(base);
  void *temp = malloc(base);
  SortInt64Ary stack;
  int64_t l, r, left, right;
  ARY_INIT(int64_t, stack, (int64_t) 5 * (log2((double) length) / 3)); 

  left  = 0;
  right = length - 1;

  while (1) {

    if (right - left < min) {
      insertionsort_min(ary + left * base, (right - left) + 1, base, smaler);
      left = right;
    }

    if (left < right) {

      l = left;
      r = right;
      memcpy(piv, ary + ((rand() % (r - l)) + l) * base, base);

      while (l < r) {
        
        while (bigger(ary + r * base, piv))
          r--; 

        while (smaler(ary + l * base, piv)) 
          l++; 

        if (equal(ary + l * base, ary + r * base)) {
          r--;

        // switch left an right
        } else if (l < r) {
          memcpy(temp, ary + l * base, base);
          memcpy(ary + l * base, ary + r * base, base);
          memcpy(ary + r * base, temp, base);
        }

      }

      ARY_PUSH(stack, right);
      right = (left > l - 1) ? left : l - 1;

    } else {
      if (stack.length > 0) {
        left = right + 1;
        ARY_PULL(stack, right);
      } else break;
    }

  }

}

/**
 * Improofed variant of QuickSort which uses insertionsort 
 * at an specific min array size, sorting max element first
 *
 * ary is a void pointer to an array
 * length is the number of elements in that array
 * base is the size of an array element
 * smaler, bigger, equal is function pointer
 * that compares two array elements the start address of
 * the elements are given as void pointers
 * 
 */
void quickinsersort_max(void *ary, 
                        int64_t length, 
                        int16_t min, 
                        int32_t base, 
                        char (*smaler)(void *, void *),
                        char (*bigger)(void *, void *),
                        char (*equal)(void *, void *)) {

  void *piv  = malloc(base);
  void *temp = malloc(base);
  SortInt64Ary stack;
  int64_t l, r, left, right;
  ARY_INIT(int64_t, stack, (int64_t) 5 * (log2((double) length) / 3)); 

  left  = 0;
  right = length - 1;

  while (1) {

    if (right - left < min) {
      insertionsort_max(ary + left * base, (right - left) + 1, base, bigger);
      left = right;
    }

    if (left < right) {

      l = left;
      r = right;
      memcpy(piv, ary + ((rand() % (r - l)) + l) * base, base);

      while (l < r) {
        
        while (smaler(ary + r * base, piv))
          r--; 

        while (bigger(ary + l * base, piv)) 
          l++; 

        if (equal(ary + l * base, ary + r * base)) {
          r--;

        // switch left an right
        } else if (l < r) {
          memcpy(temp, ary + l * base, base);
          memcpy(ary + l * base, ary + r * base, base);
          memcpy(ary + r * base, temp, base);
        }

      }

      ARY_PUSH(stack, right);
      right = (left > l - 1) ? left : l - 1;

    } else {
      if (stack.length > 0) {
        left = right + 1;
        ARY_PULL(stack, right);
      } else break;
    }

  }

}


#endif //end of CHIEFSORT
