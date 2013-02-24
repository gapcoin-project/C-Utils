/**
 * Implementation of an Array based Heap
 * methods are:
 *  heap_init(heap, max_length)
 *  heap_add(heap, element)
 *  heap_extract(heap) => element
 */
#ifndef HEAP
#define HEAP
#include "Heap.h"

/**
 * Initializes a given heap with agiven length
 *
 * note the first array element will not be used by this heap,
 * because of the speed
 */
void heap_init(Heap *heap, long max_length, char type) {
  heap->ptr = (CONTENT *) malloc(sizeof(
                CONTENT) * (max_length + 1));

  #ifdef DEBUG
    if (heap->ptr == 0)
      printf("[ERROR] couldn't allocate memory in heap_init\n");
  #endif

  heap->length = max_length;
  heap->heap_length = 0;
  heap->type = type;
}

/**
 *  switches x, and y in the given Heap
 */
void heap_switch(Heap *heap, long x, long y) {
  CONTENT tmp = heap->ptr[x];
  heap->ptr[x] = heap->ptr[y];
  heap->ptr[y] = tmp;
}

/**
 * Returns the index of parent of the given index
 */
long heap_parent(long i) {
  return i / 2;
}

/**
 * Returs the index of the left child of the given index
 */
long heap_left(long i) {
  return i * 2;
}

/**
 * Returs the index of the right child of the given index
 */
long heap_right(long i) {
  return i * 2 + 1;
}

/**
 * restors the Heap property for a max heap
 */
void max_heapify(Heap *heap, long i) {
  long l = heap_left(i);
  long r = heap_right(i);
  long max;

  if (l <= heap->heap_length
      && BIGGER(heap->ptr[l], heap->ptr[i]))
    max = l;
  else
    max = i;

  if (r <= heap->heap_length
      && BIGGER(heap->ptr[r], heap->ptr[max]))
    max = r;
  
  if (max != i) {
    heap_switch(heap, i, max);
    max_heapify(heap, max);
  }
}

/**
 * restors the Heap property for a min heap
 */
void min_heapify(Heap *heap, long i) {
  long l = heap_left(i);
  long r = heap_right(i);
  long min;

  if (l <= heap->heap_length 
      && SMALER(heap->ptr[l], heap->ptr[i]))
    min = l;
  else
    min = i;

  if (r <= heap->heap_length
      && SMALER(heap->ptr[r], heap->ptr[min]))
    min = r;
  
  if (min != i) {
    heap_switch(heap, i, min);
    min_heapify(heap, min);
  }
}

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
    while (i > 1 && SMALER(heap->ptr[heap_parent(i)], heap->ptr[i])) {
      heap_switch(heap, i, heap_parent(i));
      i = heap_parent(i);
    }
  } else if (heap->type == MINHEAP) {
    while (i > 1 && BIGGER(heap->ptr[heap_parent(i)], heap->ptr[i])) {
      heap_switch(heap, i, heap_parent(i));
      i = heap_parent(i);
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

#endif // end of Heap.c
