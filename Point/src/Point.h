/**
 * This Structur holds the a coordinate in a
 * two dimensional field
 */
#ifndef POINT
#define POINT
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  u_int32_t x, y;
} Point;

/**
 * Converts an given Point to an u_int64_t
 */
#define POINT_TO_UINT(POINT) ((((u_int64_t) POINT.y) << 32) + ((u_int64_t) POINT.x))

/**
 * Converts an given u_int64_t to an Point
 */
#define UINT_TO_POINT(UINT, POINT)      \
  POINT.x = (u_int32_t) UINT;           \
  POINT.y = (u_int32_t) (UINT >> 32);

#endif // end of Point.h

/**
 * Returns wether th given Points are equal
 */
#define POINTS_EQUAL(A, B) ((A.x == B.x) && (A.y == B.y))

/**
 * Returns wether the given Points are not equal
 */
#define POINTS_NOT_EQUAL(A, B) ((A.x != B.x) || (A.y != B.y))
