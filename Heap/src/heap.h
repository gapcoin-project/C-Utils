/**
 * Implementation of an Array based Heap
 * methods are:
 *  heap_init(content_type, heap, max_length, heap_type)
 *  heap_add(heap, element, bigger_func, smaler_func)
 *  heap_extract(heap, element, bigger_func, smaler_func)
 *  heap_search(heap, element, index, equal_func)
 */
#ifndef __HEAP_H__
#define __HEAP_H__

#include <inttypes.h>

#define MAXHEAP 'M'
#define MINHEAP 'm'

/**
 * internal Macro for ERROR  handling
 */
#define HEAP_ERR_MSG(MESSAGE)                                           \
  do {                                                                  \
    printf("[ERROR] %s in %s: %d\n", MESSAGE, __FILE__, __LINE__);      \
    exit(1);                                                            \
  } while (0)

/**
 * Define a new Heap with a given Name and Content Typ
 */
#define DEF_HEAP(TYPE, NAME)              \
  typedef struct {                        \
    TYPE *ptr;                            \
    uint64_t length;                      \
    uint64_t heap_length;                 \
    char type;                            \
  } NAME

/**
 * Initializes a given heap with a given length
 * type can be MINHEAP or MAXHEAP
 *
 * note the first array element will not be used by this heap,
 * because of the speed
 */
#define heap_init(CONTENT_TYPE, HEAP, MAX_LEN, HEAP_TYPE)                     \
  do {                                                                        \
    (HEAP).ptr = (CONTENT_TYPE *) malloc(sizeof(                              \
                  CONTENT_TYPE) * ((MAX_LEN) + 1));                           \
                                                                              \
    if ((HEAP).ptr == 0)                                                      \
      HEAP_ERR_MSG("couldn't allocate memory");                               \
                                                                              \
    (HEAP).length      = MAX_LEN;                                             \
    (HEAP).heap_length = 0;                                                   \
    (HEAP).type        = HEAP_TYPE;                                           \
  } while (0)


/**
 *  switches x, and y in the given Heap
 *
 *  note heap.ptr[0] is not an heap element
 */
#define HEAP_SWITCH(HEAP, X, Y)         \
  do {                                  \
    (HEAP).ptr[0] = (HEAP).ptr[X];      \
    (HEAP).ptr[X] = (HEAP).ptr[Y];      \
    (HEAP).ptr[Y] = (HEAP).ptr[0];      \
  } while (0)

/**
 * Returns the index of parent of the given index
 */
#define HEAP_PARENT(I) ((I) / 2)

/**
 * Returs the index of the left child of the given index
 */
#define HEAP_LEFT(I) ((I) * 2)

/**
 * Returs the index of the right child of the given index
 */
#define HEAP_RIGHT(I) ((I) * 2 + 1)

/**
 * returns wheter the given heap is not empty 
 */
#define heap_notempty(HEAP)  ((HEAP).heap_length != 0)

/**
 * returns wheter the given heap is empty 
 */
#define heap_empty(HEAP)  ((HEAP).heap_length == 0)

/**
 * restors the Heap property for a max heap
 * BIGGER must be a function
 * who takes two Heap Elements and returns
 * if the first one is bigger than the second one
 */
#define MAX_HEAPIFY(HEAP, I, BIGGER)                                  \
  do {                                                                \
    uint64_t h_i, h_l, h_r, h_max;                                    \
    h_max = I;                                                        \
                                                                      \
    do {                                                              \
      h_i = h_max;                                                    \
      h_l = HEAP_LEFT(h_i);                                           \
      h_r = HEAP_RIGHT(h_i);                                          \
                                                                      \
      if (h_l <= (HEAP).heap_length                                   \
          && BIGGER((HEAP).ptr[h_l], (HEAP).ptr[h_i]))                \
        h_max = h_l;                                                  \
      else                                                            \
        h_max = h_i;                                                  \
                                                                      \
      if (h_r <= (HEAP).heap_length                                   \
          && BIGGER((HEAP).ptr[h_r], (HEAP).ptr[h_max]))              \
        h_max = h_r;                                                  \
                                                                      \
      if (h_max != h_i)                                               \
        HEAP_SWITCH(HEAP, h_i, h_max);                                \
                                                                      \
    } while (h_max != (h_i));                                         \
  } while (0)                                             

/**
 * restors the Heap property for a min heap
 * SMALER must be a function 
 * who takes two Heap Elements and returns
 * if the first one is smaler than the second one
 */
#define MIN_HEAPIFY(HEAP, I, SMALER)                                  \
  do {                                                                \
    uint64_t h_i, h_l, h_r, h_min;                                    \
    h_min = I;                                                        \
                                                                      \
    do {                                                              \
      h_i = h_min;                                                    \
      h_l = HEAP_LEFT(h_i);                                           \
      h_r = HEAP_RIGHT(h_i);                                          \
                                                                      \
      if (h_l <= (HEAP).heap_length                                   \
          && SMALER((HEAP).ptr[h_l], (HEAP).ptr[h_i]))                \
        h_min = h_l;                                                  \
      else                                                            \
        h_min = h_i;                                                  \
                                                                      \
      if (h_r <= (HEAP).heap_length                                   \
          && SMALER((HEAP).ptr[h_r], (HEAP).ptr[h_min]))              \
        h_min = h_r;                                                  \
                                                                      \
      if (h_min != h_i)                                               \
        HEAP_SWITCH(HEAP, h_i, h_min);                                \
                                                                      \
    } while (h_min != h_i);                                           \
  } while (0)

/**
 * Extract the max or min element of the given heap, depending on the type
 *
 * BIGGER and SMALER are to functions, which
 * should take two Heap elements and return if the first one is
 * bigger or smaler as the second one
 *
 * note: it terminates the programm if you try to exctract an element
 *       from an empty Heap
 */
#define heap_extract(HEAP, VALUE, BIGGER, SMALER)             \
  do {                                                        \
    if ((HEAP).heap_length < 1)                               \
      HEAP_ERR_MSG("Heap underflow");                         \
                                                              \
    VALUE = (HEAP).ptr[1];                                    \
    (HEAP).ptr[1] = (HEAP).ptr[(HEAP).heap_length];           \
    (HEAP).heap_length--;                                     \
                                                              \
    if ((HEAP).type == MAXHEAP)                               \
      MAX_HEAPIFY(HEAP, 1, BIGGER);                           \
    else if ((HEAP).type == MINHEAP)                          \
      MIN_HEAPIFY(HEAP, 1, SMALER);                           \
    else                                                      \
      HEAP_ERR_MSG("unknowen Heap type");                     \
                                                              \
  } while (0)

/**
 * insert a given element to a given Heap
 *
 * BIGGER and SMALER are to functions, which
 * should take two Heap elements and return if the first one is
 * bigger or smaler as the second one
 *
 * note: it terminates the programm if it failed to alloc more memory
 */
#define heap_add(HEAP, VALUE, BIGGER, SMALER)                                 \
  do {                                                                        \
    if ((HEAP).length <= (HEAP).heap_length) {                                \
      (HEAP).ptr = realloc((HEAP).ptr,                                        \
                           (HEAP).length * 2 * sizeof((HEAP).ptr[0]));        \
      (HEAP).length *= 2;                                                     \
                                                                              \
      if ((HEAP).ptr == NULL)                                                 \
        HEAP_ERR_MSG("heap_add faild to alloc more memory");                  \
    }                                                                         \
                                                                              \
    (HEAP).heap_length++;                                                     \
    (HEAP).ptr[(HEAP).heap_length] = VALUE;                                   \
                                                                              \
    uint64_t h_i = (HEAP).heap_length;                                        \
                                                                              \
    if ((HEAP).type == MAXHEAP) {                                             \
                                                                              \
      while (h_i > 1 && SMALER((HEAP).ptr[HEAP_PARENT(h_i)],                  \
                             (HEAP).ptr[h_i])) {                              \
                                                                              \
        HEAP_SWITCH(HEAP, h_i, HEAP_PARENT(h_i));                             \
        h_i = HEAP_PARENT(h_i);                                               \
      }                                                                       \
                                                                              \
    } else if ((HEAP).type == MINHEAP) {                                      \
                                                                              \
      while (h_i > 1 && BIGGER((HEAP).ptr[HEAP_PARENT(h_i)],                  \
                             (HEAP).ptr[h_i])) {                              \
                                                                              \
        HEAP_SWITCH(HEAP, h_i, HEAP_PARENT(h_i));                             \
        h_i = HEAP_PARENT(h_i);                                               \
      }                                                                       \
    }                                                                         \
  } while (0)                                                                 
                                                                              
/**                                                                           
 * searches for a lement in the given Heap and
 * saves the Index in INDEX
 *
 * EQUAL should be a function
 * which indecates if two Heap elements are equal
 *
 * NOTE index will be -1 if the given element was not found
 */                                                                           
#define heap_search(HEAP, VALUE, INDEX, EQUAL)                  \
  do {                                                          \
    uint64_t h_i;                                               \
    INDEX = -1;                                                 \
                                                                \
    for (h_i = 0; h_i < (HEAP).heap_length; h_i++) {            \
      if (EQUAL((HEAP).ptr[h_i], VALUE)) {                      \
        INDEX = h_i;                                            \
        break;                                                  \
      }                                                         \
    }                                                           \
                                                                \
  } while (0)                                             
                                                          

#endif // __HEAP_H__
