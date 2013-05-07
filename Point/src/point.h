/**
 * Header of an simple two dimensional point impelementation
 * as a struct and a uint64_t which uses the higher 32 bits
 * as y and the lower as x coordianate
 */
#ifndef __POINT_H__
#define __POINT_H__
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Struct of an point in
 * a two dimensional field
 */
typedef struct {
  uint32_t x, y;
} Point;

/**
 * Converts an given Point to an uint64_t
 */
#define POINT_TO_UINT(POINT)                                    \
  ((((uint64_t) (POINT).y) << 32) + ((uint64_t) (POINT).x))

/**
 * Converts an given uint64_t to an Point
 */
#define UINT_TO_POINT(UINT, POINT)              \
  do {                                          \
    (POINT).x = (uint32_t) (UINT);              \
    (POINT).y = (uint32_t) ((UINT) >> 32);      \
  } while (0)

/**
 * Returns wether the given Points are equal
 */
#define POINTS_EQUAL(A, B) (((A).x == (B).x) && ((A).y == (B).y))

/**
 * Returns wether the given Points are not equal
 */
#define POINTS_NOT_EQUAL(A, B) (((A).x != (B).x) || ((A).y != (B).y))

#endif // __POINT_H__
