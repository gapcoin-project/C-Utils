/**
 * implementation of the default Sieve of Eratosthenes to generat 
 * a prime table including all primes till a given number starting by 2
 */
#ifndef __PRIME_TABLE_H__
#define __PRIME_TABLE_H__

#include <inttypes.h>
#include <stdlib.h>
#include <gmp.h>

/**
 * Primtable with len primes
 */
typedef struct {
  uint64_t *ptr;
  uint64_t len;
} PrimeTable;

/**
 * first 1001 primes hardcoded
 */
extern uint64_t first1001prims[];

/**
 * Stores a prime gap
 */
typedef struct {
  uint64_t start;
  uint64_t end;
  double   val;
} PrimeGap;

/**
 * stuct to store the factirisation of an given number
 */
typedef struct {
  uint64_t num;
  uint64_t *factors;
  uint64_t len;
} FactorNum;

/**
 * save a given PrimeTable to a file
 */
void save_primes(PrimeTable *primes, const char *fname);

/**
 * reads a given PrimeTable from a file
 */
PrimeTable *read_primes(const char *fname);

/**
 * 
 */
PrimeTable *gen_prime_table(uint64_t sieve_size);

/**
 * struct to store prime sequence
 *
 * a prime sequence (s) is a base composite number
 * build up out of the first x primes
 * and set M of integers with max(M) < s
 *
 * the integers (m) of M ar choosen so that
 * all primes from s till infinity are in the form of
 * n * s + m
 *
 * in fact a prime sequece with a primorial out of
 * the first n primes generates the same numbers as 
 * the sieve of erathos after sieveing the same n primes
 */
typedef struct {
  uint64_t primorial;     /* this is s form above */
  uint64_t n_primorial;   /* number of primes in  */
  uint64_t *sequence;     /* M                    */
  uint64_t len;           /* |M|                  */
  PrimeTable primes;      /* all primes < s       */
} PrimeSequence;

/**
 * free a given PrimeSequence
 */
static inline void ps_free(PrimeSequence *seq) {
  
  free(seq->sequence);
  free(seq->primes.ptr);
  free(seq);
}

/**
 * thest all primes in the given prime table to cover the given sequence
 */
char check_sequence(PrimeSequence *seq, PrimeTable *primes);

/**
 * calculates the n-th primeSequence 
 */
PrimeSequence *generate_prime_sequence(uint64_t n);

/**
 * find and returns the best prime gap in 
 */
PrimeGap *find_biggest_gap(PrimeTable *primes);

#endif /* __PRIME_TABLE_H__ */
