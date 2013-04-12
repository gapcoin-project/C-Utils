/**
 * Simple implemenation of an Iterativ quick sort algorithm witch
 * fall back on a given min size to insertion sort
 */
#ifndef __SORT_H__
#define __SORT_H__

#include <math.h>
#include <pthread.h>
#include <inttypes.h>
#include <string.h>
#include "../../Dynamic-Array/src/dynamic-array.h"
#include "../../Thread-Clients/src/thread-clients.h"

/**
 * Stack for non recursive Quicksort
 */
DEFINE_ARRAY(int64_t, SortInt64Ary);

/**
 * Struct for threadable sorting algorithms
 */
typedef struct {
  void    *ary;        /* void pointer to the array to sort */
  int64_t min;         /* the min array length to switch to insertionsort */
  int64_t base;        /* the size of an array element */
  int64_t length;      /* the array length */
  char (*smaler)(void *, void *);  /* compare functions */
  char (*bigger)(void *, void *);
  char (*equal)(void *, void *);
} QuickInsertSortThreadArgs;

#define QISTA_t QuickInsertSortThreadArgs

/**
 * Just Insertionsort
 *
 * this macro needs the TYPE of the Array Elements
 * and an smaler function which compares two array elements
 * min element first
 */
#define INSERTIONSORT_MIN(TYPE, ARY, LEN, SMALER)               \
  do {                                                          \
    int64_t is_i, is_j;                                         \
    TYPE is_temp;                                               \
                                                                \
    for (is_i = 1; is_i < (LEN); is_i++) {                      \
      is_temp = (ARY)[is_i];                                    \
      is_j = is_i - 1;                                          \
                                                                \
      while (is_j >= 0 && SMALER(is_temp, (ARY)[is_j])) {       \
        (ARY)[is_j + 1] = (ARY)[is_j];                          \
        is_j = is_j - 1;                                        \
      }                                                         \
                                                                \
      (ARY)[is_j + 1] = is_temp;                                \
    }                                                           \
  } while (0)                                                   

/**
 * Just Insertionsort
 *
 * this macro needs the TYPE of the Array Elements
 * and an bigger function which compares two array elements
 * max element first
 */
#define INSERTIONSORT_MAX(TYPE, ARY, LEN, BIGGER)               \
  do {                                                          \
    int64_t is_i, is_j;                                         \
    TYPE is_temp;                                               \
                                                                \
    for (is_i = 1; is_i < (LEN); is_i++) {                      \
      is_temp = (ARY)[is_i];                                    \
      is_j = is_i - 1;                                          \
                                                                \
      while (is_j >= 0 && BIGGER(is_temp, (ARY)[is_j])) {       \
        (ARY)[is_j + 1] = (ARY)[is_j];                          \
        is_j = is_j - 1;                                        \
      }                                                         \
                                                                \
      (ARY)[is_j + 1] = is_temp;                                \
    }                                                           \
  } while (0)                                                   

/**
 * Improofed variant of QuickSort which uses insertionsort at 
 * a specific min array size, sorting min element first
 *
 * This Macro needs the ARY element TYPE also a 
 * BIGGER, SMALER and EQL function which compares two ARY elements
 * MIN is the minimal array length to progess Quicksort 
 *     after MIN insertionsort will be used
 */
#define QUICK_INSERT_SORT_MIN(TYPE, ARY, LEN, BIGGER, SMALER, EQL, MIN)       \
  do {                                                                        \
                                                                              \
    TYPE qis_piv;                                                             \
    TYPE qis_temp;                                                            \
    SortInt64Ary qis_stack;                                                   \
    int64_t qis_l, qis_r, qis_left, qis_right;                                \
    ARY_INIT(int64_t,                                                         \
             qis_stack,                                                       \
             (int64_t) 5 * (log2((double) (LEN)) / 3));                       \
                                                                              \
    qis_left  = 0;                                                            \
    qis_right = (LEN) - 1;                                                    \
                                                                              \
    while (1) {                                                               \
                                                                              \
      if (qis_right - qis_left < (MIN)) {                                     \
        INSERTIONSORT_MIN(TYPE,                                               \
                          (ARY) + qis_left,                                   \
                          (qis_right - qis_left) + 1,                         \
                          SMALER);                                            \
                                                                              \
        qis_left = qis_right;                                                 \
      }                                                                       \
                                                                              \
      if (qis_left < qis_right) {                                             \
                                                                              \
        qis_l = qis_left;                                                     \
        qis_r = qis_right;                                                    \
        qis_piv = (ARY)[(rand() % (qis_r - qis_l)) + qis_l];                  \
                                                                              \
        while (qis_l < qis_r) {                                               \
                                                                              \
          while (BIGGER((ARY)[qis_r], qis_piv))                               \
            qis_r--;                                                          \
                                                                              \
          while (SMALER((ARY)[qis_l], qis_piv))                               \
            qis_l++;                                                          \
                                                                              \
          if (EQL((ARY)[qis_l], (ARY)[qis_r])) {                              \
            qis_r--;                                                          \
                                                                              \
          /* switch left an right */                                          \
          } else if (qis_l < qis_r) {                                         \
            qis_temp = (ARY)[qis_l];                                          \
            (ARY)[qis_l] = (ARY)[qis_r];                                      \
            (ARY)[qis_r] = qis_temp;                                          \
          }                                                                   \
                                                                              \
        }                                                                     \
                                                                              \
        ARY_PUSH(qis_stack, qis_right);                                       \
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
 * Improofed variant of QuickSort which uses insertionsort at 
 * a specific min array size, sorting max element first
 *
 * This Macro needs the ARY element TYPE also a 
 * BIGGER, SMALER and EQL function which compares two ARY elements
 * MIN is the minimal array length to progess Quicksort 
 *     after MIN insertionsort will be used
 */
#define QUICK_INSERT_SORT_MAX(TYPE, ARY, LEN, BIGGER, SMALER, EQL, MIN)       \
  do {                                                                        \
                                                                              \
    TYPE qis_piv;                                                             \
    TYPE qis_temp;                                                            \
    SortInt64Ary qis_stack;                                                   \
    int64_t qis_l, qis_r, qis_left, qis_right;                                \
    ARY_INIT(int64_t,                                                         \
             qis_stack,                                                       \
             (int64_t) 5 * (log2((double) (LEN)) / 3));                       \
                                                                              \
    qis_left  = 0;                                                            \
    qis_right = (LEN) - 1;                                                    \
                                                                              \
    while (1) {                                                               \
                                                                              \
      if (qis_right - qis_left < (MIN)) {                                     \
        INSERTIONSORT_MAX(TYPE,                                               \
                          (ARY) + qis_left,                                   \
                          (qis_right - qis_left) + 1,                         \
                          BIGGER);                                            \
                                                                              \
        qis_left = qis_right;                                                 \
      }                                                                       \
                                                                              \
      if (qis_left < qis_right) {                                             \
                                                                              \
        qis_l = qis_left;                                                     \
        qis_r = qis_right;                                                    \
        qis_piv = (ARY)[(rand() % (qis_r - qis_l)) + qis_l];                  \
                                                                              \
        while (qis_l < qis_r) {                                               \
                                                                              \
          while (SMALER((ARY)[qis_r], qis_piv))                               \
            qis_r--;                                                          \
                                                                              \
          while (BIGGER((ARY)[qis_l], qis_piv))                               \
            qis_l++;                                                          \
                                                                              \
          if (EQL((ARY)[qis_l], (ARY)[qis_r])) {                              \
            qis_r--;                                                          \
                                                                              \
          /* switch left an right */                                          \
          } else if (qis_l < qis_r) {                                         \
            qis_temp = (ARY)[qis_l];                                          \
            (ARY)[qis_l] = (ARY)[qis_r];                                      \
            (ARY)[qis_r] = qis_temp;                                          \
          }                                                                   \
                                                                              \
        }                                                                     \
                                                                              \
        ARY_PUSH(qis_stack, qis_right);                                       \
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
    SortInt64Ary qs_stack;                                                    \
    int64_t qs_l, qs_r, qs_left, qs_right;                                    \
    ARY_INIT(int64_t,                                                         \
             qs_stack,                                                        \
             (int64_t) 5 * (log2((double) (LEN)) / 3));                       \
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
        ARY_PUSH(qs_stack, qs_right);                                         \
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
                                                                              

// functions
void insertionsort_min(void *ary, 
                       int64_t length, 
                       int32_t base, 
                       char (*smaler)(void *, void *));
void insertionsort_max(void *ary, 
                       int64_t length, 
                       int32_t base, 
                       char (*bigger)(void *, void *));
void quickinsersort_min(void *ary, 
                        int64_t length, 
                        int16_t min, 
                        int32_t base, 
                        char (*smaler)(void *, void *),
                        char (*bigger)(void *, void *),
                        char (*equal)(void *, void *));
void quickinsersort_max(void *ary, 
                        int64_t length, 
                        int16_t min, 
                        int32_t base, 
                        char (*smaler)(void *, void *),
                        char (*bigger)(void *, void *),
                        char (*equal)(void *, void *));
void *t_insertionsort_min(void *ptr);
void *t_insertionsort_max(void *ptr); 
void paralell_quickinsersort_max(QISTA_t *ptr);
void paralell_quickinsersort_min(QISTA_t *ptr);
#endif // __SORT_H__
