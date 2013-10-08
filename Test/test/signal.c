#include "../src/test.h"


void my_siganl_handler(int signum) {

  (void) signum;
  TEST_MSG5("in my signal handler\n");
}


/* install signal handler */
BEVOR() {

  (void) args;
  struct sigaction new_action;
  memset(&new_action, 0, sizeof(struct sigaction));

  new_action.sa_handler = my_siganl_handler;

  sigaction(SIGTERM, &new_action, NULL);

  return NULL;
}

TEST(signal_handler) {

  (void) args;
  raise(SIGTERM);
  return NULL;

}
