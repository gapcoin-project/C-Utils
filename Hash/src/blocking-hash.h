/**
 * A shared memory hash implementation which
 * syncronizes read and write trough semaphore
 */
#ifndef __BLOCKING_HASH_H__
#define __BLOCKING_HASH_H__

#include<sys/mman.h>

/**
 * define a new bloking hash
 */
#define DEF_BHASH(TYPE, NAME)                         \
typedef struct NAME NAME;                             \
struct NAME {                                         \
  TYPE     *ptr;                                      \
  uint32_t *kys;                                      \
  const uint32_t len;                                 \
  uint32_t *n_e;                                      \
  sem_t    lock;                                      \
  /* the has constant for                             \
   * the hastfunctions */                             \
  const double h1;                                    \
  const double h2;                                    \
  TYPE tmp_e;       /* for easy sizeof */             \
} 

/**
 * checs if shared memory was succesfull initialized
 */
#define BHASH_CHECK_SHM(HASH)                                       \
do {                                                                \
  if ((HASH).ptr == (void *) -1 ||                                  \
      (HASH).kys == (void *) -1 ||                                  \
      (HASH).n_e == (void *) -1) {                                  \
                                                                    \
    perror("bhash: failed to create mmap shared memory");           \
    exit(EXIT_FAILURE);                                             \
  }                                                                 \
} while (0)
/**
 * initialize an blocking hash
 */
#define BBHASH_INIT(HASH, LEN)                                              \
do {                                                                        \
                                                                            \
  *(uint32_t *) &(HASH).len = next_prime(LEN);                              \
                                                                            \
  /* create shared memory for ptr, kys, len */                              \
  (HASH).ptr = mmap(NULL,                                                   \
                    sizeof((HASH).tmp_e) * (HASH).len,                      \
                    PROT_READ|PROT_WRITE,                                   \
                    MAP_SHARED|MAP_ANONYMOUS,                               \
                    -1,                                                     \
                    0);                                                     \
                                                                            \
                                                                            \
  (HASH).kys = mmap(NULL,                                                   \
                    sizeof(uint32_t) * (HASH).len,                          \
                    PROT_READ|PROT_WRITE,                                   \
                    MAP_SHARED|MAP_ANONYMOUS,                               \
                    -1,                                                     \
                    0);                                                     \
                                                                            \
  (HASH).n_e = mmap(NULL,                                                   \
                    sizeof(uint32_t),                                       \
                    PROT_READ|PROT_WRITE,                                   \
                    MAP_SHARED|MAP_ANONYMOUS,                               \
                    -1,                                                     \
                    0);                                                     \
                                                                            \
  BHASH_CHECK_SHM(HASH);                                                    \
                                                                            \
  *(HASH).n_e = 0;                                                          \
                                                                            \
  /* init semaphore */                                                      \
  sem_init(&(HASH).lock, 1, 1);                                             \
                                                                            \
  /* -1 and -2 are not allowed values */                                    \
  memset((HASH).kys, (uint32_t) -1, sizeof(uint32_t) * (HASH).len);         \
  memset(&(HASH).tmp_e, (uint32_t) -1, sizeof((HASH).tmp_e));               \
                                                                            \
  /** initializing h1, and h1 like Knut says */                             \
  *(double *) &(HASH).h1 = (sqrt(5.0) - 1.0) / 2.0;                         \
  *(double *) &(HASH).h2 = (sqrt(7.0) - 1.0) / 2.0;                         \
} while (0)

/**
 * frees all resources of a given hash
 * should only be called if no othre process or therad 
 * have access to the given hash
 */
#define BHASH_FREE(HASH)                                        \
do {                                                            \
  munmap((HASH).ptr, (sizeof((HASH).tmp_e) * (HASH).len);       \
  munmap((HASH).kys, (sizeof(uint32_t)     * (HASH).len);       \
  munmap((HASH).n_e, (sizeof(uint32_t));                        \
} while (0)

/**
 * Returns the decimal part of an double
 */
#define DOUBLE_DEC(DOUBLE)              \
  ((DOUBLE) - ((int32_t) (DOUBLE)))

/**
 * map key to (0..(uint32_t) -2) because -1 and -2 are reserved
 */
#define BHKEY(KEY) ((KEY) % (uint32_t) -2)

/**
 * The has function, it calculates the actual slot out of a given key,
 * and index (number of already taken slots)
 * NOTE: key has to be an integer
 */
#define BHASH_I(HASH, KEY, I)                                                 \
 ((((uint32_t) ((HASH).len * DOUBLE_DEC(((BHKEY(KEY)) * (HASH).h1)))) +       \
   ((uint32_t) (1 + (((HASH).len -1) *                                        \
                     DOUBLE_DEC(((BHKEY(KEY)) * (HASH).h2)))) * (I))) %       \
  (HASH).len)


/**
 * stores the element with the given key form the given hash
 * in E
 */
#define BHASH_GET(HASH, KEY, E)                                              \
do {                                                                         \
  sem_wait(&(HASH).lock);                                                    \
  uint32_t __hash_i__;                                                       \
                                                                             \
  for (__hash_i__ = 0;                                                       \
      __hash_i__ < (HASH).len &&                                             \
      ((HASH).kys[BHASH_I(HASH, KEY, __hash_i__)] != (BHKEY(KEY))) &&        \
      ((HASH).kys[BHASH_I(HASH, KEY, __hash_i__)] != (uint32_t) -1);         \
      __hash_i__++);                                                         \
                                                                             \
  if ((HASH).kys[BHASH_I(HASH, KEY, __hash_i__)] != (uint32_t) -1)           \
    E = (HASH).ptr[BHASH_I(HASH, KEY, __hash_i__)];                          \
                                                                             \
  sem_post(&(HASH).lock);                                                    \
                                                                             \
} while (0)

/** 
 * inserts an given element with a given key into a given hash
 * Note: ist does nothing if the has is full
 */
#define BHASH_ADD(HASH, KEY, E)                                              \
do {                                                                         \
                                                                             \
  sem_wait(&(HASH).lock);                                                    \
  uint32_t __hash_i__;                                                       \
                                                                             \
  for (__hash_i__ = 0;                                                       \
      __hash_i__ < (HASH).len &&                                             \
      ((HASH).kys[BHASH_I(HASH, KEY, __hash_i__)] != (BHKEY(KEY)))  &&       \
      ((HASH).kys[BHASH_I(HASH, KEY, __hash_i__)] != (uint32_t) -1) &&       \
      ((HASH).kys[BHASH_I(HASH, KEY, __hash_i__)] != (uint32_t) -2);         \
      __hash_i__++);                                                         \
                                                                             \
  if (__hash_i__ != (HASH).len) {                                            \
    (HASH).ptr[BHASH_I(HASH, KEY, __hash_i__)] = E;                          \
    (HASH).kys[BHASH_I(HASH, KEY, __hash_i__)] = BHKEY(KEY);                 \
    *(HASH).n_e++;                                                           \
  }                                                                          \
                                                                             \
  sem_post(&(HASH).lock);                                                    \
                                                                             \
} while (0)                                                                 
                                                                            

/**
 * Removes an element from the given hash
 */
#define BHASH_RM(HASH, KEY)                                                  \
do {                                                                         \
                                                                             \
  sem_wait(&(HASH).lock);                                                    \
  uint32_t __hash_i__;                                                       \
                                                                             \
  for (__hash_i__ = 0;                                                       \
      __hash_i__ < (HASH).len &&                                             \
      ((HASH).kys[BHASH_I(HASH, KEY, __hash_i__)] != (BHKEY(KEY))) &&        \
      ((HASH).kys[BHASH_I(HASH, KEY, __hash_i__)] != (uint32_t) -1);         \
      __hash_i__++);                                                         \
                                                                             \
  if ((HASH).kys[BHASH_I(HASH, KEY, __hash_i__)] == (BHKEY(KEY))) {          \
    (HASH).kys[BHASH_I(HASH, KEY, __hash_i__)] = (uint32_t) -2;              \
    *(HASH).n_e--;                                                           \
  }                                                                          \
                                                                             \
  sem_post(&(HASH).lock);                                                    \
                                                                             \
} while (0)


/**
 * adds an elemnt to the given hash unsing a string as key
 */
#define BHASH_ADD_STR(HASH, STR, E)                          \
  BHASH_ADD(HASH, crc32(STR, strlen(STR)), E)

/**
 * saves the wanted element in E uses a sting as key
 */
#define BHASH_GET_STR(HASH, STR, E)                          \
  BHASH_GET(HASH, crc32(STR, strlen(STR)), E)

#endif /* __BLOCKING_HASH_H__ */
