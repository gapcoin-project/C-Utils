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


// returns wheter the given heap is not empty 
#define HEAP_NOTEMPTY(HEAP)\
  HEAP.heap_length != 0

// functions
void heap_switch(Heap *heap, long x, long y);
long heap_parent(long i);
long heap_left(long i);
long heap_right(long i);
void max_heapify(Heap *heap, long i);
void min_heapify(Heap *heap, long i);
int heap_extract(Heap *heap, CONTENT *e);
int heap_add(Heap *heap, CONTENT *e);
long long heap_search(Heap *heap, CONTENT *e);

/**
 * Undefine satic Macros
 */
#undef HEAP_SWITCH

#endif // __HEAP_H__
