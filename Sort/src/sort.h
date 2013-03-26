/**
 * Simple implemenation of an Iterativ quick sort algorithm witch
 * fall back on a given min size to insertion sort
 */
#ifndef __SORT_H__
#define __SORT_H__

#include <math.h>
#include <pthread.h>
#include <inttypes.h>
#include "../../Dynamic-Array/src/dynamic-array.h"

#define PTR_AT_BASE(PTR, BASE, INDEX) ((PTR) + (BASE) * (INDEX)


/**
 * Stack for non recursive Quicksort
 */
DEFINE_ARRAY(uint64_t, SortUInt64Ary)

/**
 * Just Insertionsort
 *
 * this macro needs the TYPE of the Array Elements
 * and an smaler function which compares two array elements
 */
#define INSERTIONSORT(TYPE, ARY, LEN, SMALER)                   \
  do {                                                          \
    int is_i, is_j;                                             \
    TYPE is_temp;                                               \
                                                                \
    for (is_i = 1; is_i < LEN; is_i++) {                        \
      is_temp = ARY[is_i];                                      \
      is_j = is_i - 1;                                          \
                                                                \
      while (SMALER(is_temp, ARY[is_j]) && is_j >= 0) {         \
        ARY[is_j + 1] = ARY[is_j];                              \
        is_j = is_j - 1;                                        \
      }                                                         \
                                                                \
      ARY[is_j + 1] = is_temp;                                  \
    }                                                           \
  } while (0)                                                   


/**
 * Improofed variant of QuickSort which uses insertionsort at 
 * a specific min array size
 *
 * This Macro needs the ARY element TYPE also a 
 * BIGGER, SMALER and EQL function which compares two ARY elements
 * MIN is the minimal array length to progess Quicksort 
 *     after MIN insertionsort will be used
 */
#define QUICK_INSERT_SORT(TYPE, ARY, LEN, BIGGER, SMALER, EQL, MIN)           \
  do {                                                                        \
                                                                              \
    TYPE qis_piv;                                                             \
    TYPE qis_temp;                                                            \
    SortUInt64Ary qis_stack;                                                  \
    uint64_t qis_l, qis_r, qis_left, qis_right;                               \
    ARY_INIT(uint64_t,                                                        \
             qis_stack,                                                       \
             (uint64_t) 5 * (log2((double) (LEN)) / 3));                      \
                                                                              \
    qis_left  = 0;                                                            \
    qis_right = (LEN) - 1;                                                    \
                                                                              \
    while (1) {                                                               \
                                                                              \
      if (qis_right - qis_left < MIN) {                                       \
        INSERTIONSORT(TYPE,                                                   \
                      ARY + qis_left,                                         \
                      (qis_right - qis_left) + 1,                             \
                      SMALER);                                                \
                                                                              \
        qis_left = qis_right;                                                 \
      }                                                                       \
                                                                              \
      if (qis_left < qis_right) {                                             \
                                                                              \
        qis_l = qis_left;                                                     \
        qis_r = qis_right;                                                    \
        qis_piv = ARY[(rand() % (qis_r - qis_l)) + qis_l];                    \
                                                                              \
        while (qis_l < qis_r) {                                               \
                                                                              \
          while (BIGGER(ARY[qis_r], qis_piv))                                 \
            qis_r--;                                                          \
                                                                              \
          while (SMALER(ARY[qis_l], qis_piv))                                 \
            qis_l++;                                                          \
                                                                              \
          if (EQL(ARY[l], ARY[r])) {                                          \
            qis_r--;                                                          \
                                                                              \
          /* switch left an right */                                          \
          } else if (qis_l < qis_r) {                                         \
            qis_temp = ARY[qis_l];                                            \
            ARY[qis_l] = ARY[qis_r];                                          \
            ARY[qis_r] = qis_temp;                                            \
          }                                                                   \
                                                                              \
        }                                                                     \
                                                                              \
        ARY_PUSH(uint64_t, qis_stack, qis_right);                             \
        qis_right = (qis_left > qis_l - 1) ? qis_left : qis_l - 1;            \
                                                                              \
      } else {                                                                \
        if (qis_stack.length > 0) {                                           \
          qis_left = qis_right + 1;                                           \
          ARY_PULL(qis_stack, qis_right);                                     \
        } else break;                                                         \
      }                                                                       \
                                                                              \
    }                                                                         \
                                                                              \
  } while (0)                                                                 
                                                                              

/**
 * Just QuickSort, without recursion
 *
 * This Macro needs the ARY element TYPE also a 
 * BIGGER, SMALER and EQL function which compares two ARY elements
 */
#define QUICKSORT(TYPE, ARY, LEN, BIGGER, SMALER, EQL)                        \
  do {                                                                        \
                                                                              \
    TYPE qs_piv;                                                              \
    TYPE qs_temp;                                                             \
    SortUInt64Ary qs_stack;                                                   \
    uint64_t qs_l, qs_r, qs_left, qs_right;                                   \
    ARY_INIT(uint64_t,                                                        \
             qs_stack,                                                        \
             (uint64_t) 5 * (log2((double) (LEN)) / 3));                      \
                                                                              \
    qs_left  = 0;                                                             \
    qs_right = (LEN) - 1;                                                     \
                                                                              \
    while (1) {                                                               \
                                                                              \
      if (qs_left < qs_right) {                                               \
                                                                              \
        qs_l = qs_left;                                                       \
        qs_r = qs_right;                                                      \
        qs_piv = ARY[(rand() % (qs_r - qs_l)) + qs_l];                        \
                                                                              \
        while (qs_l < qs_r) {                                                 \
                                                                              \
          while (BIGGER(ARY[qs_r], qs_piv))                                   \
            qs_r--;                                                           \
                                                                              \
          while (SMALER(ARY[qs_l], qs_piv))                                   \
            qs_l++;                                                           \
                                                                              \
          if (EQL(ARY[l], ARY[r])) {                                          \
            qs_r--;                                                           \
                                                                              \
          /* switch left an right */                                          \
          } else if (qs_l < qs_r) {                                           \
            qs_temp = ARY[qs_l];                                              \
            ARY[qs_l] = ARY[qs_r];                                            \
            ARY[qs_r] = qs_temp;                                              \
          }                                                                   \
                                                                              \
        }                                                                     \
                                                                              \
        ARY_PUSH(uint64_t, qs_stack, qs_right);                               \
        qs_right = (qs_left > qs_l - 1) ? qs_left : qs_l - 1;                 \
                                                                              \
      } else {                                                                \
        if (qs_stack.length > 0) {                                            \
          qs_left = qs_right + 1;                                             \
          ARY_PULL(qs_stack, qs_right);                                       \
        } else break;                                                         \
      }                                                                       \
                                                                              \
    }                                                                         \
                                                                              \
  } while (0)                                                                 
                                                                              


/**
 * Struct for handling paralell sorting
 */
typedef struct {
  CHIEFSORT_TYPE *ary;
  int length;
  int threads;
  int min_insertionsort;
} ChiefSortParalell;

// functions
void *serialchiefsort(void *arg);
void *paralellchiefsort(void *arg);
void chiefsort(CHIEFSORT_TYPE *ary, int length, int min);
void insertionsort(CHIEFSORT_TYPE *ary, int length);
void quicksort(CHIEFSORT_TYPE *ary, int length);
void paralellsort(CHIEFSORT_TYPE *ary, int length, int min_insertionsort, int threads);
#endif // __SORT_H__
