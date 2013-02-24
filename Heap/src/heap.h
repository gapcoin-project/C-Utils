/**
 * Implementation of an Array based Heap
 * methods are:
 *  heap_init(heap, max_length)
 *  heap_add(heap, element)
 *  heap_extract(heap) => element
 */
#ifndef HEAP_HEADER
#define HEAP_HEADER

#define MAXHEAP 'M'
#define MINHEAP 'm'
#ifndef ASTAR_HEADER
#include "Point.h"
/**
 * Implementation of an AStart Point 
 * (just additional saves it f and g value)
 */
typedef struct {
  Point p;
  double  f, g;
} APoint;

// Heap init
#define CONTENT APoint
#define BIGGER(X, Y) X.f > Y.f
#define SMALER(X, Y) X.f < Y.f
#define EQUAL(X, Y) X.p.x == Y.p.x && X.p.y == Y.p.y
#endif

#ifndef CONTENT
#include "Point.h"
#define CONTENT Point
#endif
#ifndef BIGGER
#define BIGGER(X, Y) X > Y
#endif
#ifndef SMALER
#define SMALER(X, Y) X < Y
#endif
#ifndef EQUAL
#define EQUAL(X, Y) X == Y
#endif

/**
 * Heap Structure
 */
typedef struct {
  CONTENT *ptr;
  long length;
  long heap_length;
  char type;
} Heap;

// returns wheter the given heap is not empty 
#define HEAP_NOTEMPTY(HEAP)\
  HEAP.heap_length != 0

// functions
void heap_init(Heap *heap, long max_length, char type);
void heap_switch(Heap *heap, long x, long y);
long heap_parent(long i);
long heap_left(long i);
long heap_right(long i);
void max_heapify(Heap *heap, long i);
void min_heapify(Heap *heap, long i);
int heap_extract(Heap *heap, CONTENT *e);
int heap_add(Heap *heap, CONTENT *e);
long long heap_search(Heap *heap, CONTENT *e);
#endif // end of HEAP_HEADER
