#include "../src/test.h"

TEST(basic_test) {

  (void) args;
  
  ASSERT_TRUE(1, "this should work\n");
  ASSERT_TRUE(0, "this should not work\n");

  return NULL;
}

TEST(working_test) {

  (void) args;
  
  ASSERT_TRUE(1, "this should work\n");
  ASSERT_FALSE(0, "this should also work\n");

  return NULL;
}

TEST(fail_test) {

  (void) args;
  
  ASSERT_FAIL("this should fail\n");

  return NULL;
}

TEST(kill_test) {

  (void) args;

  raise(SIGKILL);

  return NULL;
}


TEST(sigterm_test) {

  (void) args;

  raise(SIGTERM);

  return NULL;
}

TEST(sighup_test) {
  
  (void) args;

  raise(SIGHUP);

  return NULL;
}

TEST(sigint_test) {
  
  (void) args;

  raise(SIGINT);

  return NULL;
}
        
TEST(sigquit_test) {
  
  (void) args;

  raise(SIGQUIT);

  return NULL;
}
        
TEST(sigill_test) {
  
  (void) args;

  raise(SIGILL);

  return NULL;
}
        
TEST(sigabrt_test) {
  
  (void) args;

  raise(SIGABRT);

  return NULL;
}
        
TEST(sigfpe_test) {
  
  (void) args;

  raise(SIGFPE);

  return NULL;
}
        
TEST(sigsegv_test) {
  
  (void) args;

  raise(SIGSEGV);

  return NULL;
}
        
TEST(sigpipe_test) {
  
  (void) args;

  raise(SIGPIPE);

  return NULL;
}
        
TEST(sigalarm_test) {
  
  (void) args;

  raise(SIGALRM);

  return NULL;
}
