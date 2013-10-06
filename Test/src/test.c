/**
 *  Simple testing framework
 */
#ifndef __TEST__
#define __TEST__

#include "test.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../../String/src/string.h"

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

  /* let test fail */
  ARY_AT(tunit.tests, tunit.i).failed = 1;
  
  /* find the caled signal */
  Signal sig = { -1, NULL, NULL };
  unsigned int i;

  for (i = 0; i < sizeof(signals) / sizeof(signals[0]); i++) {
    if (signals[i].signum == signum) {
      sig = signals[i];
      break;
    }
  }
  
  TEST_MSG("recieved signal %s: %s in", sig.name, sig.description);
  show_backtrace();

  /* call old signal handler */
  ARY_AT(ARY_AT(tunit.tests, tunit.i).signals, i).sa_handler(signum);

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
 * prints a stacktrace for the current stack
 */
void show_backtrace() {
  if (tunit.debug) {
    TEST_MSG("DEBUG STACKTRACE\n");
  } else {
    TEST_MSG("NON DEBUG STACKTRACE\n");
  }
}

/**
 * test if we can use gdb to the stack trace
 */
char can_use_gdb() {
  
  /* check if gdb is installed */
  char *path = getenv("PATH");

  if (path == NULL)
    return 0;

  const char **dirs = (const char **) split(path, ":");
  
  int i = 0;
  char buffer[256];
  char found = 0;
  for (i = 0; dirs[i] != NULL && !found; i++) {
    
    sprintf(buffer, "%s/gdb", dirs[i]);

    if (!access(buffer, F_OK))
      found = 1;
  }

  if (!found)
    return 0;

  /* check if permittion to ptrace is granded */
  int fd = open("/proc/sys/kernel/yama/ptrace_scope", O_RDONLY);

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
 * Main function which runs the tests
 */
int main(int argc, char *argv[]) {

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

  /* run bevor_all function */
  if (tunit.bevor_all != NULL)
    tunit.bevor_all_res = tunit.bevor_all();

  /* runn all tests */
  for (tunit.i = 0; tunit.i < ARY_LEN(tunit.tests); tunit.i++) {

    /* run test in new process */
    pid_t pid = fork();

    /* child */
    if (pid == 0) {

      void *bevor_res = NULL;
      void *test_res  = NULL;

      Test *test = &ARY_AT(tunit.tests, tunit.i);
      
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

    } else if (pid < 0) {
      perror("Failed to creat child prozess");
      exit(EXIT_FAILURE);
    } else {
      
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

        } else {
          TEST_MSG("[II] test %s exited with unknowen reason\n", 
                   ARY_AT(tunit.tests, tunit.i).test_name);
        }
      }
    }
  }
  
  if (tunit.after_all != NULL)
    tunit.after_all(tunit.bevor_all_res);

  return 0;
}

#endif /* __TEST__ */
