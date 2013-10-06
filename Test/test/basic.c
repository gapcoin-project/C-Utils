#include "../src/test.h"

TEST(basic_test) {

  (void) args;
  
  ASSERT_TRUE(1, "this should work\n");
  ASSERT_TRUE(0, "this should not work\n");

  return NULL;
}

TEST(kill_test) {

  (void) args;

  kill(getpid(), SIGKILL);

  return NULL;
}
