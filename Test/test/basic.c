#include "../src/test.h"

TEST(basic_test, "Basic Test") {

  (void) args;
  
  ASSERT_TRUE(1, "this should work\n");
  ASSERT_TRUE(0, "this should not work\n");

  return NULL;
}

TEST(kill_test, "Kill Test") {

  (void) args;

  kill(getpid(), SIGKILL);

  return NULL;
}
