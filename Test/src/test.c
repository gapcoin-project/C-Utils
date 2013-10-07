/**
 *  Simple testing framework
 */
#ifndef __TEST__
#define __TEST__

#include "test.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <execinfo.h>
#include "../../String/src/string.h"
#include "../../Time-Diff/src/time-diff.h"

/**
 * init tunit first;
 */
void __attribute__ ((constructor(101))) init_tunit() {

  memset(&tunit, 0, sizeof(TestUnit));
  ARY_INIT(Test, tunit.tests, 8);
}


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
  { SIGALRM, "SIGALRM", "Timer signal from alarm(2)" },
  { SIGTERM, "SIGTERM", "Termination signal" },
};

/**
 * the test signal handler which prints a stacktrace
 * and calls the old signal handler
 */
void test_signal_handler(int signum) {

  /* let test fail with error */
  ARY_AT(tunit.tests, tunit.i).failed = 1;
  ARY_AT(tunit.tests, tunit.i).error  = 1;
  
  /* find the caled signal */
  Signal sig = { -1, NULL, NULL };
  unsigned int i;

  for (i = 0; i < sizeof(signals) / sizeof(signals[0]); i++) {
    if (signals[i].signum == signum) {
      sig = signals[i];
      break;
    }
  }
  
  TEST_MSG("[EE] recieved signal %s: %s\n", sig.name, sig.description);
  show_backtrace();

  void (*old_handler) (int);
  old_handler = ARY_AT(ARY_AT(tunit.tests, tunit.i).signals, i).sa_handler;

  if (old_handler == SIG_DFL)
    exit(EXIT_FAILURE);

  /* call old signal handler */
  if (old_handler != SIG_IGN)
    old_handler(signum);
}

/**
 * install teh signal handlers for a given test
 */
static inline void install_signal_hanlders(Test *test) {
  
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
         "  -d  --debug\t\tif you want to debug this application\n");
}

/**
 * Returns if the given args conatinig the given arg
 */
static inline char has_arg(int argc, 
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
static inline char *get_arg(int argc, 
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
static inline void get_backtrace() {

  void *buffer[100];
  int n = backtrace(buffer, 100);

  char **strings = backtrace_symbols(buffer, n);

  int i;
  for (i = 0; i < n; i++)
    if (n < 10)
      TEST_MSG("#%d %s\n", i, strings[i]);
    else if (i < 10)
      TEST_MSG("#%d  %s\n", i, strings[i]);
    else
      TEST_MSG("#%d %s\n", i, strings[i]);

  free(strings);
}

/**
 * get gdb backtrace
 */
static inline void get_gdb_backtrace() {

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

  int i;

  int size = fread(buffer, 1, 1020, trace);

  for (i = 0; size > 0 && i < 100; i++) {

    TEST_MSG("%s", buffer);
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
  int fd = open("/proc/sys/kernel/yama/ptrace_scope", O_RDONLY);
  char buffer[2];

  if (fd == -1)
    return 0;

  int size = read(fd, buffer, 1);

  if (size != 1 || buffer[0] != '0')
    return 0;
    
  return 1;
}

/**
 * prints a smal start up message 
 */
static inline void info_messages(int argc, char *argv[]) {
  
  TEST_MSG("[II] Test Framework started with %" PRIu64" test\n", 
           ARY_LEN(tunit.tests));

  if (tunit.debug)
    TEST_MSG("[II] running in Debug mode\n");

  if (tunit.log)
    TEST_MSG("[II] logging to %s\n", get_arg(argc, argv, "-l", "--log"));

  TEST_MSG("\n");
}

/**
 * initializes the Test Framework
 */
static inline void init(int argc, char *argv[]) {

  /* save file name */
  tunit.fname = argv[0];

  if (has_arg(argc, argv, "-h", "--help")) {
    print_usage(argv[0]);
    exit(EXIT_SUCCESS);
  }

  tunit.verbose = !has_arg(argc, argv, "-q", "--quiet");
  tunit.debug   =  has_arg(argc, argv, "-d", "--debug");
  tunit.log     =  has_arg(argc, argv, "-l", "--log");
  
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
    ARY_AT(tunit.tests, tunit.i).failed = 1;
    
    /* if test was KILLED */
    if (WIFSIGNALED(child_status) && WTERMSIG(child_status) == SIGKILL) {
      TEST_MSG("[EE] test %s got killed\n", 
               ARY_AT(tunit.tests, tunit.i).test_name);

    } else if (!WIFSIGNALED(child_status)) {
      TEST_MSG("[II] test %s exited with unknowen reason\n", 
               ARY_AT(tunit.tests, tunit.i).test_name);
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

  TEST_MSG("[II] Test %s needed %.3LF seconds, exited %s",
           test->test_name,
           seconds,
           exit_status);

  TEST_MSG("\n\n");
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

  TEST_MSG("[II] Test Framework %s finished %" PRIu64 " tests\n",
           name,
           ARY_LEN(tunit.tests));

  TEST_MSG("[II] Succes: %" PRIu64 "\n", ARY_LEN(tunit.tests) - n_failurs);
  TEST_MSG("[II] Failed: %u\n", n_failurs);
  TEST_MSG("[II] Errors: %u\n", n_errors);

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

    Test *test = &ARY_AT(tunit.tests, tunit.i);

    TEST_MSG("[II] starting test: %s\n", test->test_name); 
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

#endif /* __TEST__ */
