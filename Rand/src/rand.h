#ifndef __RAND_H_
#define __RAND_H_

#include <inttypes.h>

static inline uint32_t rand32(uint32_t *x) {
  *x ^= *x << 13;
  *x ^= *x >> 17;
  *x ^= *x << 5;
  return *x;
}

/* random value */
typedef struct {
  uint32_t x, y, z, w;
} rand128_t; 

static inline rand128_t *new_rand128(uint32_t seed) {
  rand128_t *rand = (rand128_t *) malloc(sizeof(rand128_t));

  rand->x = rand32(&seed);
  rand->y = rand32(&seed);
  rand->z = rand32(&seed);
  rand->w = rand32(&seed);

  return rand;
}
                      
static inline uint32_t rand128(rand128_t *rand) {
                               
  uint32_t t = rand->x ^ (rand->x << 11);
  rand->x = rand->y; rand->y = rand->z; rand->z = rand->w;
  rand->w ^= (rand->w >> 19) ^ t ^ (t >> 8);
                                      
  return rand->w;
}

#define rand128_range(rand, start, end) ((rand128(rand) % ((end) - (start))) + (start))
#define rand32_range(rand, start, end) ((rand32(rand) % ((end) - (start))) + (start))

#endif /* __RAND_H_ */
