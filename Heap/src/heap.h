/**
 * Implementation of an Array based Heap
 * methods are:
 *  heap_init(heap, max_length)
 *  heap_add(heap, element)
 *  heap_extract(heap) => element
 */
#ifndef __HEAP_H__
#define __HEAP_H__

#define MAXHEAP 'M'
#define MINHEAP 'm'

/**
 * Define a new Heap with a given Name and Content Typ
 */
#define DEF_HEAP(TYPE, NAME)              \
  typedef struct {                        \
    TYPE *ptr;                            \
    long length;                          \
    long heap_length;                     \
    char type;                            \
  } NAME

/**
 * Initializes a given heap with a given length
 * type can be MINHEAP or MAXHEAP
 *
 * note the first array element will not be used by this heap,
 * because of the speed
 */
#define heap_init(CONTENT_TYPE, HEAP, MAX_LEN, HEAP_TYPE) {                   \
  do {                                                                        \
    (HEAP).ptr = (CONTENT_TYPE *) malloc(sizeof(                              \
                  CONTENT_TYPE) * ((MAX_LEN) + 1));                           \
                                                                              \
    if ((HEAP).ptr == 0) {                                                    \
      printf("[ERROR] couldn't allocate memory in heap_init at %s: %d\n",     \
             __FILE__,                                                        \
             __LINE__);                                                       \
      exit(1);                                                                \
    }                                                                         \
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
    (HEAP).ptr[0] = (HEAP).ptr[x];      \
    (HEAP).ptr[x] = (HEAP).ptr[y];      \
    (HEAP).ptr[y] = (HEAP).ptr[0];      \
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
 * BIGGER must be a macro or function pointer
 * who takes two Heap Elements and returns
 * if the first one is bigger than the second one
 */
#define MAX_HEAPIFY(HEAP, I, BIGGER)                      \
  do {                                                    \
    long l, r, max;                                       \
                                                          \
    do {                                                  \
      l = HEAP_LEFT(I);                                   \
      r = HEAP_RIGHT(I);                                  \
                                                          \
      if (l <= (HEAP).heap_length                         \
          && BIGGER((HEAP).ptr[l], (HEAP).ptr[I]))        \
        max = l;                                          \
      else                                                \
        max = I;                                          \
                                                          \
      if (r <= heap->heap_length                          \
          && BIGGER((HEAP).ptr[r], (HEAP).ptr[max]))      \
        max = r;                                          \
                                                          \
      if (max != (I))                                     \
        HEAP_SWITCH((heap), I, max);                      \
                                                          \
    } while (max != (I));                                 \
  } while (0)                                             

/**
 * restors the Heap property for a min heap
 * SMALER must be a macro or function pointer
 * who takes two Heap Elements and returns
 * if the first one is smaler than the second one
 */
#define MIN_HEAPIFY(HEAP, I, SMALER)                      \
  do {                                                    \
    long l, r, min;                                       \
                                                          \
    do {                                                  \
      l = HEAP_LEFT(I);                                   \
      r = HEAP_RIGHT(I);                                  \
                                                          \
      if (l <= (HEAP).heap_length                         \
          && SMALER((HEAP).ptr[l], (HEAP).ptr[I]))        \
        min = l;                                          \
      else                                                \
        min = I;                                          \
                                                          \
      if (r <= (HEAP).heap_length                         \
          && SMALER((HEAP).ptr[r], (HEAP).ptr[min]))      \
        min = r;                                          \
                                                          \
      if (min != (I))                                     \
        HEAP_SWITCH((heap), I, min);                      \
                                                          \
    } while (min != (I));                                 \
  } while (0)

/**
 * Extract the max or min element of the given heap, depending on the type
 * returns -1 if it failed to extract the element
 */
int heap_extract(Heap *heap, CONTENT *e) {
  if (heap->heap_length < 1)
    return -1;

  *e = heap->ptr[1];
  heap->ptr[1] = heap->ptr[heap->heap_length];
  heap->heap_length--;
  
  if (heap->type == MAXHEAP)
    max_heapify(heap, 1);
  else if (heap->type == MINHEAP)
    min_heapify(heap, 1);
  else
    return -1;

  return 1;
}

/**
 * insert a given element to a given Heap
 * returns 1 on sucess and -1 when failed
 */
int heap_add(Heap *heap, CONTENT *e) {
  if (heap->length <= heap->heap_length ) {
    heap->ptr = (CONTENT *) realloc(heap->ptr, heap->length *  2 * sizeof(CONTENT)); 
    heap->length *= 2;

    #ifdef VERBOSE
      #ifndef VERBOSE_ONELINE
        printf("Heap Overflow expected: length: %lu   heap_length %lu\n", heap->length, heap->heap_length);
      #endif
    #endif
  }

  heap->heap_length++;
  heap->ptr[heap->heap_length] = *e;

  long i = heap->heap_length;

  if (heap->type == MAXHEAP) {
    while (i > 1 && SMALER(heap->ptr[HEAP_PARENT(i)], heap->ptr[i])) {
      HEAP_SWITCH(heap, i, HEAP_PARENT(i));
      i = HEAP_PARENT(i);
    }
  } else if (heap->type == MINHEAP) {
    while (i > 1 && BIGGER(heap->ptr[HEAP_PARENT(i)], heap->ptr[i])) {
      HEAP_SWITCH(heap, i, HEAP_PARENT(i));
      i = HEAP_PARENT(i);
    }
  }

  return 1;
}

/**
 * searches for a lement in the given Heap and returns the index
 */
long long heap_search(Heap *heap, CONTENT *e) {
  long long i;

  for (i = 0; i < heap->heap_length; i++) {
    if (EQUAL(heap->ptr[i], (*e))) return i;
  }

  return -1;
}

// functions
int heap_extract(Heap *heap, CONTENT *e);
int heap_add(Heap *heap, CONTENT *e);
long long heap_search(Heap *heap, CONTENT *e);

/**
 * Undefine satic Macros
 */
#undef HEAP_SWITCH
#undef HEAP_PARENT
#undef HEAP_LEFT
#undef HEAP_RIGHT

#endif // __HEAP_H__
