/**
 *  Simple testing framework
 */
#ifndef __TEST_H__
#define __TEST_H__

#include "../../Dynamic-Array/src/dynamic-array.h"
#include "../../String/src/string.h"
#include "../../Time-Diff/src/time-diff.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <execinfo.h>

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
void *bevor_all(void);                                                      \
                                                                            \
/* add the function to the test unit */                                     \
void __attribute__((constructor(102))) add_bevor_all() {                    \
  tunit.bevor_all = bevor_all;                                              \
}                                                                           \
                                                                            \
/* start of the function implementation */                                  \
void *bevor_all(void)


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
void after(void *args);                                                     \
                                                                            \
/* add the function to the test unit */                                     \
void __attribute__((constructor(102))) add_after() {                        \
  tunit.after = after;                                                      \
}                                                                           \
                                                                            \
/* start of the function implementation */                                  \
void after(void *args)


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
#define TEST_MSG1(...)                                        \
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
 * print or log a message
 * useable like printf
 */
#define TEST_MSG2(...)                                        \
do {                                                          \
                                                              \
  if (tunit.verbose > 1)                                      \
    dprintf(STDOUT_FILENO, __VA_ARGS__);                      \
                                                              \
  if (tunit.log)                                              \
    dprintf(tunit.log_fd, __VA_ARGS__);                       \
                                                              \
} while (0)

/**
 * print or log a message
 * useable like printf
 */
#define TEST_MSG3(...)                                        \
do {                                                          \
                                                              \
  if (tunit.verbose > 2)                                      \
    dprintf(STDOUT_FILENO, __VA_ARGS__);                      \
                                                              \
  if (tunit.log)                                              \
    dprintf(tunit.log_fd, __VA_ARGS__);                       \
                                                              \
} while (0)

/**
 * print or log a message
 * useable like printf
 */
#define TEST_MSG4(...)                                        \
do {                                                          \
                                                              \
  if (tunit.verbose > 3)                                      \
    dprintf(STDOUT_FILENO, __VA_ARGS__);                      \
                                                              \
  if (tunit.log)                                              \
    dprintf(tunit.log_fd, __VA_ARGS__);                       \
                                                              \
} while (0)

/**
 * print or log a message
 * useable like printf
 */
#define TEST_MSG5(...)                                        \
do {                                                          \
                                                              \
  if (tunit.verbose > 4)                                      \
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
  TEST_MSG3("[EE] in %s:", ARY_AT(tunit.tests, tunit.i).test_name);         \
  TEST_MSG3(" " __VA_ARGS__);                                               \
  TEST_MSG3("\n");                                                          \
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
 * if a test depends on the return aguments of an other test
 * (it re runs and returns the arguments form the given test)
 */
#define DEPENDS_ON_ARGS(TEST)                               \
  TEST(tunit.bevor ? tunit.bevor(tunit.bevor_all_res) :     \
                                 tunit.bevor_all_res)

/**
 * if a test depends on the previous execution of other tests
 * Note the named test will be reexecuted
 */
#define DEPENDS_ON1(TEST)                                                 \
do {                                                                      \
  if (tunit.after != NULL) {                                              \
    tunit.after(TEST(tunit.bevor ? tunit.bevor(tunit.bevor_all_res) :     \
                                   tunit.bevor_all_res));                 \
  } else {                                                                \
    TEST(tunit.bevor ? tunit.bevor(tunit.bevor_all_res) :                 \
                       tunit.bevor_all_res);                              \
  }                                                                       \
} while (0)

#define DEPENDS_ON2(T1, T2)   \
do {                          \
  DEPENDS_ON1(T1);            \
  DEPENDS_ON1(T2);            \
} while (0)

#define DEPENDS_ON3(T1, T2, T3)   \
do {                              \
  DEPENDS_ON1(T1);                \
  DEPENDS_ON1(T2);                \
  DEPENDS_ON1(T3);                \
} while (0)

#define DEPENDS_ON4(T1, T2, T3, T4)   \
do {                                  \
  DEPENDS_ON1(T1);                    \
  DEPENDS_ON1(T2);                    \
  DEPENDS_ON1(T3);                    \
  DEPENDS_ON1(T4);                    \
} while (0)

#define DEPENDS_ON5(T1, T2, T3, T4, T5)   \
do {                                      \
  DEPENDS_ON1(T1);                        \
  DEPENDS_ON1(T2);                        \
  DEPENDS_ON1(T3);                        \
  DEPENDS_ON1(T4);                        \
  DEPENDS_ON1(T5);                        \
} while (0)

#define DEPENDS_ON6(T1, T2, T3, T4, T5, T6)   \
do {                                          \
  DEPENDS_ON1(T1);                            \
  DEPENDS_ON1(T2);                            \
  DEPENDS_ON1(T3);                            \
  DEPENDS_ON1(T4);                            \
  DEPENDS_ON1(T5);                            \
  DEPENDS_ON1(T6);                            \
} while (0)

#define DEPENDS_ON7(T1, T2, T3, T4, T5, T6, T7)   \
do {                                              \
  DEPENDS_ON1(T1);                                \
  DEPENDS_ON1(T2);                                \
  DEPENDS_ON1(T3);                                \
  DEPENDS_ON1(T4);                                \
  DEPENDS_ON1(T5);                                \
  DEPENDS_ON1(T6);                                \
  DEPENDS_ON1(T7);                                \
} while (0)

#define DEPENDS_ON8(T1, T2, T3, T4, T5, T6, T7, T8)   \
do {                                                  \
  DEPENDS_ON1(T1);                                    \
  DEPENDS_ON1(T2);                                    \
  DEPENDS_ON1(T3);                                    \
  DEPENDS_ON1(T4);                                    \
  DEPENDS_ON1(T5);                                    \
  DEPENDS_ON1(T6);                                    \
  DEPENDS_ON1(T7);                                    \
  DEPENDS_ON1(T8);                                    \
} while (0)

#define DEPENDS_ON9(T1, T2, T3, T4, T5, T6, T7, T8, T9)   \
do {                                                      \
  DEPENDS_ON1(T1);                                        \
  DEPENDS_ON1(T2);                                        \
  DEPENDS_ON1(T3);                                        \
  DEPENDS_ON1(T4);                                        \
  DEPENDS_ON1(T5);                                        \
  DEPENDS_ON1(T6);                                        \
  DEPENDS_ON1(T7);                                        \
  DEPENDS_ON1(T8);                                        \
  DEPENDS_ON1(T9);                                        \
} while (0)

/**
 * A short trick to define a variable args macro
 */
#define DEPENDS_ONX(X, T1, T2, T3, T4, T5, T6, T7, T8, T9, FUNC, ...) FUNC
#define DEPENDS_ON(...)                          \
        DEPENDS_ONX(, ##__VA_ARGS__,             \
                      DEPENDS_ON9(__VA_ARGS__),  \
                      DEPENDS_ON8(__VA_ARGS__),  \
                      DEPENDS_ON7(__VA_ARGS__),  \
                      DEPENDS_ON6(__VA_ARGS__),  \
                      DEPENDS_ON5(__VA_ARGS__),  \
                      DEPENDS_ON4(__VA_ARGS__),  \
                      DEPENDS_ON3(__VA_ARGS__),  \
                      DEPENDS_ON2(__VA_ARGS__),  \
                      DEPENDS_ON1(__VA_ARGS__))

/*********************************************************************
 * include main and source                                           *
 * (we have to do this to use the same static tunit with in test.c   *
 *  and within the actual test source file)                          *
 *********************************************************************/

/**
 * init tunit first;
 */
void __attribute__ ((constructor(101))) init_tunit() {

  memset(&tunit, 0, sizeof(TestUnit));
  ARY_INIT(Test, tunit.tests, 8);
}

/**
 * Macro to acces the current running test
 */
#define CUR_TEST ARY_AT(tunit.tests, tunit.i)

/**
 * returns if the current test failed
 */
#define CUR_TEST_FAILED CUR_TEST.failed

/**
 * structur to hold infomation about an signal
 */
typedef struct {
  int signum;
  const char *name;
  const char *description;
} Signal;

/**
 * array of catched signals in the tests
 */
static Signal signals[] = { 
  { SIGHUP,  "SIGHUP",  "Hangup detected on controlling terminal "
                        "or death of controlling process" },
  { SIGINT,  "SIGINT",  "Interrupt from keyboard" },
  { SIGQUIT, "SIGQUIT", "Quit from keyboard" },
  { SIGILL,  "SIGILL",  "Illegal Instruction" },
  { SIGABRT, "SIGABRT", "Abort signal from abort(3)" },
  { SIGFPE,  "SIGFPE",  "Floating point exception" },
  { SIGSEGV, "SIGSEGV", "Invalid memory reference" },
  { SIGPIPE, "SIGPIPE", "Broken pipe: write to pipe with no readers" },
  { SIGTERM, "SIGTERM", "Termination signal" },
};

/**
 * the test signal handler which prints a stacktrace
 * and calls the old signal handler
 */
void test_signal_handler(int signum) {

  /* let test fail with error */
  CUR_TEST.failed = 1;
  CUR_TEST.error  = 1;
  
  /* find the caled signal */
  Signal sig = { -1, NULL, NULL };
  unsigned int i;

  for (i = 0; i < sizeof(signals) / sizeof(signals[0]); i++) {
    if (signals[i].signum == signum) {
      sig = signals[i];
      break;
    }
  }
  
  TEST_MSG2("[EE] %s recieved signal %s: %s\n", 
            CUR_TEST.test_name, 
            sig.name, 
            sig.description);

  show_backtrace();

  void (*old_handler) (int);
  old_handler = ARY_AT(CUR_TEST.signals, i).sa_handler;

  /* reset signal hanler to default */
  if (old_handler == SIG_DFL) {
    signal(signum, SIG_DFL);
    raise(signum);

  /* call old signal handler */
  } else if (old_handler != SIG_IGN)
    old_handler(signum);
}

/**
 * install teh signal handlers for a given test
 */
static void install_signal_hanlders(Test *test) {
  
  unsigned int i;
  struct sigaction new_action, old_action;
  memset(&new_action, 0, sizeof(struct sigaction));

  new_action.sa_handler = test_signal_handler;

  for (i = 0; i < sizeof(signals) / sizeof(signals[0]); i++) {

    sigaction(signals[i].signum, &new_action, &old_action);
    ARY_ADD(test->signals, old_action);
  }
}

/**
 * print usage
 */
static inline void print_usage(char *name) {
  printf("%s Test Framework:\n", name);
  printf("  -h  --help\t\tprint these message\n"
         "  -q  --quiet\t\tbe quiet\n"
         "  -l  --log [FILE]\tlog qutput to FILE\n"
         "  -d  --debug\t\tif you want to debug this application\n"
         "  -v  --verbose [1,2,3,4,5] set the verbose level\n"
         "        1: only summary\n"
         "        2: 1 + errors\n"
         "        3: 2 + failurs\n"
         "        4: 3 + one-line test info (default)\n"
         "        5: 4 + extendet test info\n");
}

/**
 * Returns if the given args conatinig the given arg
 */
static char has_arg(int argc, 
                    char *argv[], 
                    const char *short_arg, 
                    const char *long_arg) {

  int i;
  for (i = 1; i < argc; i++) {
    if (!strcmp(argv[i], short_arg) ||
        !strcmp(argv[i], long_arg)) {
      
      return 1;
    }
  }

  return 0;
}

/**
 * Returns the given argument arg
 */
static char *get_arg(int argc, 
                     char *argv[], 
                     const char *short_arg, 
                     const char *long_arg) {

  int i;
  for (i = 1; i < argc - 1; i++) {
    if (!strcmp(argv[i], short_arg) ||
        !strcmp(argv[i], long_arg)) {
      
      return argv[i + 1];
    }
  }

  return NULL;
}

/**
 * get normal backtrace
 */
static void get_backtrace() {

  void *buffer[100];
  int n = backtrace(buffer, 100);

  char **strings = backtrace_symbols(buffer, n);

  int i;

  if (CUR_TEST.error) {
    for (i = 0; i < n; i++)
      if (n < 10)
        TEST_MSG2("#%d %s\n", i, strings[i]);
      else if (i < 10)
        TEST_MSG2("#%d  %s\n", i, strings[i]);
      else
        TEST_MSG2("#%d %s\n", i, strings[i]);

  } else {
    for (i = 0; i < n; i++)
      if (n < 10)
        TEST_MSG3("#%d %s\n", i, strings[i]);
      else if (i < 10)
        TEST_MSG3("#%d  %s\n", i, strings[i]);
      else
        TEST_MSG3("#%d %s\n", i, strings[i]);
  }

  free(strings);
}

/**
 * get gdb backtrace
 */
static void get_gdb_backtrace() {

  char buffer[1024];

  char name_buf[512];
  name_buf[readlink("/proc/self/exe", name_buf, 511)]=0;

  sprintf(buffer, 
          "gdb --batch -n -ex thread -ex bt %s %d 2>/dev/null|grep \"#\"",
          name_buf, 
          getpid());
  
  FILE *trace = popen(buffer, "r");

  if (trace <= (FILE *) 0)
    return get_backtrace();

  int i, k;
  char abort = 0;

  int size = fread(buffer, 1, 1020, trace);

  for (i = 0; size > 0 && i < 100 && !abort; i++) {

    for (k = 1; k < size; k++) {
      if (buffer[k - 1] == '\n' && buffer[k] != '#') {
        abort = 1;
        break;
      }
    }

    if (CUR_TEST.error && tunit.verbose > 1)
      write(STDOUT_FILENO, buffer, k);
    else if (tunit.verbose > 2)
      write(STDOUT_FILENO, buffer, k);

    if (tunit.log)
      write(tunit.log_fd, buffer, k);

    size = fread(buffer, 1, 1020, trace);
  }
  fclose(trace);
}

/**
 * prints a stacktrace for the current stack
 */
void show_backtrace() {
  if (tunit.debug) {
    get_backtrace();
  } else {
    get_gdb_backtrace();
  }
}

/**
 * test if we can use gdb to the stack trace
 */
char can_use_gdb() {
  
  /* check if gdb is installed */
  char path[] = "/usr/bin/gdb";

  if (access(path, F_OK))
    return 0;

  /* check if permittion to ptrace is granded */
  char ptrace_scope[] = "/proc/sys/kernel/yama/ptrace_scope";

  int fd = open(ptrace_scope, O_RDONLY);
  char buffer[2];

  if (fd == -1)
    return 0;

  int size = read(fd, buffer, 1);
  close(fd);

  if (size != 1)
    return 0;

  /* try to set ptrace_scope to 1 */
  if (buffer[0] != '0') {
    if (access(ptrace_scope, W_OK))
      return 0;
    
    fd = open(ptrace_scope, O_WRONLY);

    if (fd == -1)
      return 0;

    if (write(fd, "1\n", 2) != 2) {
      close(fd);
      return 0;
    }

    close(fd);
  }
    
  return 1;
}

/**
 * prints a smal start up message 
 */
static inline void info_messages(int argc, char *argv[]) {
  
  TEST_MSG5("[II] Test Framework started with %" PRIu64" test\n", 
           ARY_LEN(tunit.tests));

  if (tunit.debug)
    TEST_MSG5("[II] running in Debug mode\n");

  if (tunit.log)
    TEST_MSG5("[II] logging to %s\n", get_arg(argc, argv, "-l", "--log"));

  TEST_MSG5("\n");
}

/**
 * initializes the Test Framework
 */
static inline void init(int argc, char *argv[]) {

  /* save file name */
  tunit.fname = argv[0];

  /* standart verbose level */
  tunit.verbose = 4;

  if (has_arg(argc, argv, "-h", "--help")) {
    print_usage(argv[0]);
    exit(EXIT_SUCCESS);
  }

  if (has_arg(argc, argv, "-v", "--verbose"))
    tunit.verbose = atoi(get_arg(argc, argv, "-v", "--verbose"));
  
  tunit.verbose = has_arg(argc, argv, "-q", "--quiet") ? 0 : tunit.verbose;
  tunit.debug   = has_arg(argc, argv, "-d", "--debug");
  tunit.log     = has_arg(argc, argv, "-l", "--log");

  if (tunit.log)
    tunit.log_fd = open(get_arg(argc, argv, "-l", "--log"), O_CREAT|O_WRONLY, 00777);

  if (tunit.log_fd == -1) {
    perror("Failed to oepen log file");
    exit(EXIT_FAILURE);
  }

  /* check if we can use gdb for backtrace */
  if (!can_use_gdb())
    tunit.debug = 1;

  info_messages(argc, argv);
}

/**
 * runns the current test
 * and exits (the child prozess)
 */
static inline void run_test(Test *test) {

  void *bevor_res = NULL;
  void *test_res  = NULL;
  
  /* run bevor with resiult of bevor_all */
  if (tunit.bevor != NULL)
    bevor_res = tunit.bevor(tunit.bevor_all_res);
  
  /* install signal handler */
  install_signal_hanlders(test);

  /* run test */
  test_res = test->func((tunit.bevor ? bevor_res : tunit.bevor_all_res));

  /* do clean up */
  if (tunit.after != NULL)
    tunit.after(test_res);

  /* exit test environment */
  exit(EXIT_SUCCESS);
}

/**
 * waits for the child prozess with the given pid
 * to execute te current test
 */
static inline void wait_for(pid_t pid) {
      
  int child_status;
  waitpid(pid, &child_status, 0);
  
  /* if not exited normaly */
  if (!WIFEXITED(child_status)) {

    /* le the test fail */
    CUR_TEST.failed = 1;
    
    /* if test was KILLED */
    if (WIFSIGNALED(child_status) && WTERMSIG(child_status) == SIGKILL) {
      TEST_MSG2("[EE] test %s got killed\n", 
               CUR_TEST.test_name);

    } else if (!WIFSIGNALED(child_status)) {
      TEST_MSG2("[II] test %s exited with unknowen reason\n", 
               CUR_TEST.test_name);
    }
  }
}

/**
 * prints and or logs information about the current finished test
 */
static inline void do_test_info(Test *test) {
  
  char *exit_status = test->failed ? "with failur" : "succesfull";

  long double seconds = (long double) test->time.tv_sec + 
                        (long double) test->time.tv_usec / 1000000.0L;

  TEST_MSG5("[II] Test %s needed %.3LF seconds, exited %s",
           test->test_name,
           seconds,
           exit_status);

  TEST_MSG5("\n\n");
}

/**
 * prints a summery after all test ar runed
 */
static inline void do_test_summary(char *name) {
  
  unsigned int n_failurs, n_errors, i;

  for (i = 0, n_failurs = 0, n_errors = 0; i < ARY_LEN(tunit.tests); i++) {

    if (ARY_AT(tunit.tests, i).failed)
      n_failurs++;

    if (ARY_AT(tunit.tests, i).error)
      n_errors++;

  }

  if (n_errors)
    TEST_MSG2("\n");
  else if (n_failurs)
    TEST_MSG3("\n");
  else
    TEST_MSG4("\n");

  TEST_MSG1("[II] Test Framework %s finished %" PRIu64 " tests\n",
           name,
           ARY_LEN(tunit.tests));

  TEST_MSG1("[II] Succes: %" PRIu64 "\n", ARY_LEN(tunit.tests) - n_failurs);
  TEST_MSG1("[II] Failed: %u\n", n_failurs);
  TEST_MSG1("[II] Errors: %u\n", n_errors);

}

/**
 * copys the Test array into a shared memory segement
 */
static inline void make_test_shared() {
  
  
  void *ptr = mmap(NULL,                                             
                   ARY_LEN(tunit.tests) * sizeof(Test),
                   PROT_READ|PROT_WRITE,                             
                   MAP_SHARED|MAP_ANONYMOUS,                         
                   -1,                                               
                   0);                                               

  if (ptr == MAP_FAILED) {
    perror("failed to create shared memory for the test!");
    exit(EXIT_FAILURE);
  }

  memcpy(ptr, tunit.tests.ptr, ARY_LEN(tunit.tests) * sizeof(Test));
  free(tunit.tests.ptr);

  tunit.tests.ptr = ptr;
}
/**
 * Main function which runs the tests
 */
int main(int argc, char *argv[]) {

  init(argc, argv);

  /* stop time */
  gettimeofday(&tunit.start, NULL);

  make_test_shared();

  /* run bevor_all function */
  if (tunit.bevor_all != NULL)
    tunit.bevor_all_res = tunit.bevor_all();

  /* runn all tests */
  for (tunit.i = 0; tunit.i < ARY_LEN(tunit.tests); tunit.i++) {

    Test *test = &CUR_TEST;

    TEST_MSG4("[II] starting test: %s\n", test->test_name); 
    gettimeofday(&test->start, NULL);

    /* run test in new process */
    pid_t pid = fork();

    /* child */
    if (pid == 0) {

      run_test(test);

    } else if (pid < 0) {
      perror("Failed to creat child prozess");
      exit(EXIT_FAILURE);

    /* parrent */
    } else {
      wait_for(pid);
    }

    gettimeofday(&test->end, NULL);
    timeval_subtract(&test->time, &test->end, &test->start);

    /* print info about the finished test */
    do_test_info(test);
  }
  
  if (tunit.after_all != NULL)
    tunit.after_all(tunit.bevor_all_res);

  gettimeofday(&tunit.end, NULL);
  timeval_subtract(&tunit.time, &tunit.end, &tunit.start);

  /* print summery */
  do_test_summary(argv[0]);

  return 0;
}

#endif /* __TEST_H__ */
