/**
 *  Simple testing framework
 */
#ifndef __TEST_H__
#define __TEST_H__

#include "../../Dynamic-Array/src/dynamic-array.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>

/**
 * prints a stacktrace for the current stack
 */
void show_backtrace();

/**
 * an Array for sigaction structs
 */
DEF_ARY(struct sigaction, SIGAry);

/**
 * A simple test case
 *
 * Note all test functions will be run in a forked child process,
 * and bevor the test some signal handler are installed to log,
 * errors, (the prevoius nstalled signal handerl are called afterwards)
 * so if you have to install own signal hander to this in the bevor function
 * or make sure the old signal handler is called, otherwise you dont get test 
 * output if something failed
 */
typedef struct {
  void   *(*func) (void *); /* the test function */
  char   failed;            /* indecates if this test failed */
  char   error;             /* indecates if this test failed do to an error */
  const  char *test_name;   /* the name of the test */
  SIGAry signals;           /* the old siganl handlers of this */
  struct timeval start;
  struct timeval end;
  struct timeval time;
} Test;

/**
 * function pointer array for the tests
 */
DEF_ARY(Test, TestAry);

/**
 * The Test unit structur
 *
 * +-------------+-----------------------------------------------------------+
 * | bevor_all   | the function is the first function ever called,           |
 * |             | its return value is passed to all bevor functions.        |
 * |             |                                                           |
 * | bevor       | this function is called bevor every test, it gets the     |
 * |             | return value from, the bevor_all function,                |
 * |             | the retun value from this will the be given to the        |
 * |             | test function                                             |
 * |             |                                                           |
 * | after       | this function gets the return value from the previous     |
 * |             | executed test function, and should process clean up after |
 * |             | a test.                                                   |
 * |             | Note: if the test failed it gets the pointer form bevor.  |
 * |             |                                                           |
 * | after_all   | This function is executed bevor the exit of the test, it  |
 * |             | gets the return value from bevor_all, and soud do a last  |
 * |             | clean up.                                                 |
 * |             |                                                           |
 * | debug       | indecates if the test programm should be debuged, if so,  |
 * |             | a it cant use gdb to print the stack trace.               |
 * |             |                                                           |
 * | verbose     | print output to stdout, standart is true.                 |
 * |             |                                                           |
 * | log         | logs the output to a log file, standtart is false.        |
 * +-------------+-----------------------------------------------------------+
 *
 *                                                                           
 * Note: all test functions, and the after function are executed in a new    
 *       process (the test and the after function are executed in the      
 *       same new process)                                                  
 *
 * Note: all the given argumnets ar named args, so dont use that name if you
 *       dont use the args
 */                                                                         
typedef struct {
  const char *fname;
  void  *(*bevor_all) (void);
  void  *(*bevor) (void *);
  void  (*after) (void *);
  void  (*after_all) (void *);
  void  *bevor_all_res;
  TestAry tests;
  char  debug;
  char  verbose;
  char  log;
  int   log_fd;
  uint64_t i;      /* index of the current progressing test */
  struct timeval start;
  struct timeval end;
  struct timeval time;
} TestUnit;

/**
 * the Test Unit
 */
static TestUnit tunit;

/**
 * macro for adding the bevor_all function
 */
#define BEVOR_ALL()                                                         \
/* declaration */                                                           \
void *bevor_all(void *args);                                                \
                                                                            \
/* add the function to the test unit */                                     \
void __attribute__((constructor(102))) add_bevor_all() {                    \
  tunit.bevor_all = bevor_all;                                              \
}                                                                           \
                                                                            \
/* start of the function implementation */                                  \
void *bevor_all(void *args)


/**
 * macro for adding the bevor function
 */
#define BEVOR()                                                             \
/* declaration */                                                           \
void *bevor(void *args);                                                    \
                                                                            \
/* add the function to the test unit */                                     \
void __attribute__((constructor(102))) add_bevor() {                        \
  tunit.bevor = bevor;                                                      \
}                                                                           \
                                                                            \
/* start of the function implementation */                                  \
void *bevor(void *args)


/**
 * macro for adding the after function
 */ 
#define AFTER()                                                             \
/* declaration */                                                           \
void *after(void *args);                                                    \
                                                                            \
/* add the function to the test unit */                                     \
void __attribute__((constructor(102))) add_after() {                        \
  tunit.after = after;                                                      \
}                                                                           \
                                                                            \
/* start of the function implementation */                                  \
void *after(void *args)


/**
 * macro for adding the after_all function
 */
#define AFTER_ALL()                                                         \
/* declaration */                                                           \
void after_all(void *args);                                                 \
                                                                            \
/* add the function to the test unit */                                     \
void __attribute__((constructor(102))) add_after_all() {                    \
  tunit.after_all = after_all;                                              \
}                                                                           \
                                                                            \
/* start of the function implementation */                                  \
void after_all(void *args)



/**
 * macro for adding a test case
 */
#define TEST(NAME)                                                          \
/* declaration */                                                           \
void *NAME(void *args);                                                     \
                                                                            \
/**                                                                         \
 * add the function to the test unit                                        \
 */                                                                         \
void __attribute__ ((constructor(102))) add_test_##NAME() {                 \
                                                                            \
  Test test;                                                                \
  memset(&test, 0, sizeof(Test));                                           \
  test.func   = NAME;                                                       \
  test.test_name = #NAME;                                                   \
  ARY_INIT(struct sigaction, test.signals, 10);                             \
                                                                            \
  ARY_ADD(tunit.tests, test);                                               \
}                                                                           \
                                                                            \
/* start of the function implementation */                                  \
void *NAME(void *args)


/**
 * print or log a message
 * useable like printf
 */
#define TEST_MSG(...)                                         \
do {                                                          \
                                                              \
  if (tunit.verbose)                                          \
    dprintf(STDOUT_FILENO, __VA_ARGS__);                      \
                                                              \
  if (tunit.log)                                              \
    dprintf(tunit.log_fd, __VA_ARGS__);                       \
                                                              \
} while (0)

/**
 * assert that the condition is false 
 * if its true it logs or prints a stacktrace and 
 * if given formated message
 */
#define ASSERT_FALSE(CONDITION, ...)                                        \
if (CONDITION) {                                                            \
  TEST_MSG("[EE] in %s: ", ARY_AT(tunit.tests, tunit.i).test_name);         \
  TEST_MSG(__VA_ARGS__);                                                    \
  show_backtrace();                                                         \
                                                                            \
  /* let test fail */                                                       \
  ARY_AT(tunit.tests, tunit.i).failed = 1;                                  \
                                                                            \
  /* return the args given by bevor */                                      \
  return args;                                                              \
}                                       

/**
 * assert that the condition is false 
 * if its true it logs or prints a stacktrace and 
 * if given formated message
 */
#define ASSERT_TRUE(CONDITION, ...) ASSERT_FALSE(!(CONDITION), __VA_ARGS__)

/**
 * let the test fail with a backtrace and a given formated message
 */
#define ASSERT_FAIL(...) ASSERT_FALSE(1, __VA_ARGS__)

/**
 * include main 
 * (we have to do this to use the same static tunit with in test.c
 *  and within the actual test source file)
 */
#include "test.c"

#endif /* __TEST_H__ */
