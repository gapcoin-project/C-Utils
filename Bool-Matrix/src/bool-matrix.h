/**
 * Header for an Bool Matrix
 * Minimizes spaces if only True or False is Neccesary
 */
#ifndef __BOOL_MATRIX_H__
#define __BOOL_MATRIX_H__

#include <sys/types.h>
#include <inttypes.h>

typedef struct {
  uint8_t **ptr;
  uint32_t width, height;
} BoolMatrix;

#define BIT0 1
#define BIT1 2
#define BIT2 4
#define BIT3 8
#define BIT4 16
#define BIT5 32
#define BIT6 64
#define BIT7 128

/**
 * Inits Matrix with False
 */
#define INIT_MATRIX(MATRIX, WIDTH, HEIGHT)                                    \
  do {                                                                        \
    uint32_t w = WIDTH;                                                       \
    uint32_t h = HEIGHT;                                                      \
    if (w <= 0) w = 1;                                                        \
    if (h <= 0) h = 1;                                                        \
    (MATRIX).ptr = (uint8_t **) malloc(sizeof(uint8_t *) * w);                \
                                                                              \
    uint32_t i;                                                               \
    for (i = 0; i < w; i++) {                                                 \
      (MATRIX).ptr[i] = (uint8_t *) calloc(h / 8 + 1, sizeof(uint8_t));       \
    }                                                                         \
    (MATRIX).width = w;                                                       \
    (MATRIX).height = (h / 8 + 1) * 8;                                        \
  } while (0)                                                            

/**
 * Doubles the size in X direction
 */
#define MATRIX_GROW_X(MATRIX)                                                 \
 do {                                                                         \
    (MATRIX).ptr = realloc((MATRIX).ptr,                                      \
                           (MATRIX).width * 2 * sizeof(uint8_t *));           \
    uint32_t i;                                                               \
    for (i = (MATRIX).width; i < (MATRIX).width * 2; i++) {                   \
      (MATRIX).ptr[i] = calloc((MATRIX).height / 8, sizeof(uint8_t));         \
    }                                                                         \
    (MATRIX).width *= 2;                                                      \
  } while (0)
    
/**
 * Doubles the size in Y direction
 */
#define MATRIX_GROW_Y(MATRIX)                                             \
  do {                                                                    \
    uint32_t i;                                                           \
    uint32_t height = (MATRIX).height / 8;                                \
    uint32_t newheight = ((MATRIX).height / 8) * 2;                       \
    for (i = 0; i < (MATRIX).width; i++) {                                \
      (MATRIX).ptr[i] = realloc((MATRIX).ptr[i],                          \
                                newheight * sizeof(uint8_t));             \
      memset((MATRIX).ptr[i] + height, 0, height);                        \
    }                                                                     \
    (MATRIX).height *= 2;                                                 \
  } while (0)

/**
 * Sets the given Possition
 */
#define MATRIX_SET(MATRIX, X, Y)                              \
  do {                                                        \
    while ((MATRIX).width <= (X)) {                           \
      MATRIX_GROW_X(MATRIX)                                   \
    }                                                         \
    while ((MATRIX).height <= (Y)) {                          \
      MATRIX_GROW_Y(MATRIX)                                   \
    }                                                         \
    switch ((Y) % 8) {                                        \
      case 0 : (MATRIX).ptr[X][(Y) / 8] |= BIT0; break;       \
      case 1 : (MATRIX).ptr[X][(Y) / 8] |= BIT1; break;       \
      case 2 : (MATRIX).ptr[X][(Y) / 8] |= BIT2; break;       \
      case 3 : (MATRIX).ptr[X][(Y) / 8] |= BIT3; break;       \
      case 4 : (MATRIX).ptr[X][(Y) / 8] |= BIT4; break;       \
      case 5 : (MATRIX).ptr[X][(Y) / 8] |= BIT5; break;       \
      case 6 : (MATRIX).ptr[X][(Y) / 8] |= BIT6; break;       \
      case 7 : (MATRIX).ptr[X][(Y) / 8] |= BIT7; break;       \
    }                                                         \
  } while (0)
    
/**
 * Unsets the given Possition
 */
#define MATRIX_UNSET(MATRIX, X, Y)                                \
  do {                                                            \
    while (MATRIX.width <= (X)) {                                 \
      MATRIX_GROW_X(MATRIX)                                       \
    }                                                             \
    while (MATRIX.height <= (Y)) {                                \
      MATRIX_GROW_Y(MATRIX)                                       \
    }                                                             \
    switch ((Y) % 8) {                                            \
      case 0 : (MATRIX).ptr[X][(Y) / 8] &= ~BIT0; break;          \
      case 1 : (MATRIX).ptr[X][(Y) / 8] &= ~BIT1; break;          \
      case 2 : (MATRIX).ptr[X][(Y) / 8] &= ~BIT2; break;          \
      case 3 : (MATRIX).ptr[X][(Y) / 8] &= ~BIT3; break;          \
      case 4 : (MATRIX).ptr[X][(Y) / 8] &= ~BIT4; break;          \
      case 5 : (MATRIX).ptr[X][(Y) / 8] &= ~BIT5; break;          \
      case 6 : (MATRIX).ptr[X][(Y) / 8] &= ~BIT6; break;          \
      case 7 : (MATRIX).ptr[X][(Y) / 8] &= ~BIT7; break;          \
    }                                                             \
  } while (0)

/**
 * Sets Both 
 */
#define MATRIX_SET_BOTH(MATRIX, X, Y)       \
  do {                                      \
    MATRIX_SET(MATRIX, X, Y)                \
    MATRIX_SET(MATRIX, Y, X)                \
  } while (0)

/**
 * Unsets Both 
 */
#define MATRIX_UNSET_BOTH(MATRIX, X, Y)       \
  do {                                        \
    MATRIX_UNSET(MATRIX, X, Y)                \
    MATRIX_UNSET(MATRIX, Y, X)                \
  } while (0)

/**
 * Sets all entrys to zero
 */
#define MATRIX_CLEAR(MATRIX)                          \
  do {                                                \
    uint32_t i;                                       \
    for (i = 0; i < MATRIX.width; i++) {              \
      memset(MATRIX.ptr[i], 0, MATRIX.height / 8);    \
    }                                                 \
  } while (0)

/**
 * Sets specific entrys to zero
 */
#define MATRIX_CLEAR_PART(MATRIX, N)                          \
  do {                                                        \
    while ((MATRIX).width <= (N)) {                           \
      MATRIX_GROW_X(MATRIX)                                   \
    }                                                         \
    while ((MATRIX).height <= (N)) {                          \
      MATRIX_GROW_Y((MATRIX))                                 \
    }                                                         \
    uint32_t i;                                               \
    for (i = 0; i <= (N); i++) {                              \
      memset((MATRIX).ptr[i], 0, (MATRIX).height / 8);        \
    }                                                         \
  } while (0)

/**
 * Sets the main diagonal
 */
#define MATRIX_SET_MAIN_DIAGONAL(MATRIX)                                      \
  do {                                                                        \
    uint32_t i, min = ((MATRIX).width < (MATRIX).height) ? (MATRIX).width     \
                                                          : (MATRIX).height;  \
    for (i = 0; i < min; i++) {                                               \
      MATRIX_SET(MATRIX, i, i);                                               \
    }                                                                         \
  } while (0)

/**
 * Clears and sets the main diagonal
 */
#define MATRIX_SET_CLEAR_MAIN_DIAGONAL(MATRIX)            \
  do {                                                    \
    MATRIX_CLEAR(MATRIX)                                  \
    MATRIX_SET_MAIN_DIAGONAL(MATRIX)                      \
  } while (0)

/**
 * Sets the main diagonal
 */
#define MATRIX_SET_MAIN_DIAGONAL_PART(MATRIX, N)                              \
  do {                                                                        \
    for (i = 0; i <= (N); i++) {                                              \
      MATRIX_SET(MATRIX, i, i);                                               \
    }                                                                         \
  } while (0)

/**
 * returns wheter the given entry is setted or not
 */
#define IS_MATRIX_SETTED(MATRIX, X, Y)                  \
  ((Y % 8 == 0 && (MATRIX.ptr[X][Y / 8] & BIT0)) ||     \
   (Y % 8 == 1 && (MATRIX.ptr[X][Y / 8] & BIT1)) ||     \
   (Y % 8 == 2 && (MATRIX.ptr[X][Y / 8] & BIT2)) ||     \
   (Y % 8 == 3 && (MATRIX.ptr[X][Y / 8] & BIT3)) ||     \
   (Y % 8 == 4 && (MATRIX.ptr[X][Y / 8] & BIT4)) ||     \
   (Y % 8 == 5 && (MATRIX.ptr[X][Y / 8] & BIT5)) ||     \
   (Y % 8 == 6 && (MATRIX.ptr[X][Y / 8] & BIT6)) ||     \
   (Y % 8 == 7 && (MATRIX.ptr[X][Y / 8] & BIT7)))
   

#define MATRIX_AT(MATRIX, X, Y) IS_MATRIX_SETTED(MATRIX, X, Y)

/**
 * calculate the path Matrix WITHOUT setting the main diagonal
 * NOTE this will fail if the main diagonal is not setted
 * so onely use this if you are 100% shure that the main diagonal 
 * is allready setted
 * 
 * This only works on n-X-n matrixes
 * Because the Matrix spaces resevation and realy used part
 * can different you have to give the n
 */
#define MATRIX_CALC_PATHS_WITHOUT(MATRIX, N)                                  \
  do {                                                                        \
    uint32_t k, i, j;                                                         \
    for (k = 0; k <= N; k++) {                                                \
      for (i = 0; i <= N; i++) {                                              \
        for (j = 0; j <= N; j++) {                                            \
          if ((MATRIX_AT(MATRIX, i, k) && MATRIX_AT(MATRIX, k, j))) {         \
            MATRIX_SET(MATRIX, i , j)                                         \
          }                                                                   \
        }                                                                     \
      }                                                                       \
    }                                                                         \
  } while (0)

#endif // __BOOL_MATRIX_H__
